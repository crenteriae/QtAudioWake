#include "tonegenerator.h"
#include <QAudioFormat>
#include <QMediaDevices>
#include <QtMath>
#include <qaudiodevice.h>
#include <qmediadevices.h>

namespace {
constexpr int kSilencePaddingMs = 10;
constexpr int kFadeMs = 25;
constexpr double kMaxAmplitude = 32767.0;
} // namespace

ToneGenerator::ToneGenerator(QObject *parent) : QObject(parent) {
    initAudioSink();
    generateTone();
}

ToneGenerator::~ToneGenerator() = default;

void ToneGenerator::initAudioSink() {
    QAudioFormat format;
    format.setSampleRate(m_sampleRate);
    format.setChannelCount(1);
    format.setSampleFormat(QAudioFormat::Int16);

    QAudioDevice device = QMediaDevices::defaultAudioOutput();
    if (device.isNull()) {
        qWarning("No audio output device available");
        return;
    }
    if (!device.isFormatSupported(format)) {
        qWarning("Audio format not supported");
        format = device.preferredFormat();
    }

    m_audioSink.reset(new QAudioSink(device, format));

    connect(m_audioSink.data(), &QAudioSink::stateChanged, this,
            &ToneGenerator::handleStateChanged);
}

void ToneGenerator::handleStateChanged(QAudio::State state) {
    if (state == QAudio::IdleState || state == QAudio::StoppedState) {
        if (m_isPlaying) {
            m_isPlaying = false;
            m_audioSink->stop();
            emit playbackFinished();
        }
    }
}

void ToneGenerator::setFrequency(int frequency) {
    if (m_frequency != frequency) {
        m_frequency = frequency;
        generateTone();
    }
}

void ToneGenerator::setDuration(int durationMs) {
    if (m_durationMs != durationMs) {
        m_durationMs = durationMs;
        generateTone();
    }
}

void ToneGenerator::setVolume(float volume) {
    float clamped = qBound(0.0f, volume, 1.0f);
    if (!qFuzzyCompare(m_volume, clamped)) {
        m_volume = clamped;
        generateTone();
    }
}

bool ToneGenerator::isPlaying() const { return m_isPlaying; }

void ToneGenerator::generateTone() {
    const int toneSamples = (m_sampleRate * m_durationMs) / 1000;

    if (toneSamples < 1) {
        m_audioData.clear();
        return;
    }

    // Audio devices need a few ms of silence upon initializing
    // Will otherwise produce a clicking sound
    const int silencePadding = (m_sampleRate * kSilencePaddingMs) / 1000;
    const int totalSamples = silencePadding + toneSamples + silencePadding;

    m_audioData.resize(totalSamples * sizeof(qint16));
    qint16 *data = reinterpret_cast<qint16 *>(m_audioData.data());

    memset(data, 0, totalSamples * sizeof(qint16)); // Fill buffer with silence

    // Generate tone in the middle section
    const double angularFreq = 2.0 * M_PI * m_frequency / m_sampleRate;
    const double amplitude = kMaxAmplitude * m_volume;

    int fadeSamples = qMin((m_sampleRate * kFadeMs) / 1000, toneSamples / 4);
    fadeSamples = qMax(fadeSamples, 1);

    qint16 *toneStart = data + silencePadding;

    for (int i = 0; i < toneSamples; ++i) {
        double envelope = 1.0;

        if (i < fadeSamples) {
            envelope = 0.5 * (1.0 - qCos(M_PI * i / fadeSamples));
        } else if (i >= toneSamples - fadeSamples) {
            int fadePos = i - (toneSamples - fadeSamples);
            envelope = 0.5 * (1.0 + qCos(M_PI * fadePos / fadeSamples));
        }

        toneStart[i] =
            static_cast<qint16>(amplitude * envelope * qSin(angularFreq * i));
    }

    toneStart[0] = 0;
    toneStart[toneSamples - 1] = 0;
}

void ToneGenerator::playTone() {
    if (m_isPlaying || m_audioData.isEmpty()) {
        return;
    }

    m_isPlaying = true;

    m_buffer.reset(new QBuffer(&m_audioData));
    m_buffer->open(QIODevice::ReadOnly);

    m_audioSink->start(m_buffer.data());

    emit tonePlayed();
}

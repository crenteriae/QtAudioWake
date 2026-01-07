#ifndef TONEGENERATOR_H
#define TONEGENERATOR_H

#include <QAudioSink>
#include <QBuffer>
#include <QByteArray>
#include <QObject>
#include <QScopedPointer>

class ToneGenerator : public QObject
{
    Q_OBJECT

public:
    explicit ToneGenerator(QObject *parent = nullptr);
    ~ToneGenerator();

    void setFrequency(int frequency);
    void setDuration(int durationMs);
    void setVolume(float volume);

    bool isPlaying() const;

public slots:
    void playTone();

signals:
    void tonePlayed();
    void playbackFinished();

private slots:
    void handleStateChanged(QAudio::State state);

private:
    void generateTone();
    void initAudioSink();

    int m_frequency = 19000;
    int m_durationMs = 50;
    float m_volume = 0.3f;
    int m_sampleRate = 48000;
    bool m_isPlaying = false;

    QByteArray m_audioData;
    QScopedPointer<QBuffer> m_buffer;
    QScopedPointer<QAudioSink> m_audioSink;
};

#endif // TONEGENERATOR_H

/*
 * QtAudioWake
 * Copyright (C) 2026 Cesar Renteria
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#ifndef TONEGENERATOR_H
#define TONEGENERATOR_H

#include <QAudioSink>
#include <QBuffer>
#include <QByteArray>
#include <QObject>
#include <QScopedPointer>

class ToneGenerator : public QObject {
    Q_OBJECT

  public:
    explicit ToneGenerator(QObject *parent = nullptr);
    ~ToneGenerator();

    void setFrequency(int frequency);
    void setDuration(int durationMs);
    void setVolume(float volume);

    int getFrequency();
    int getDurationMs();
    float getVolume();

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

    int m_frequency = 22000;
    int m_durationMs = 50;
    float m_volume = 0.3f;
    int m_sampleRate = 48000;
    bool m_isPlaying = false;

    QByteArray m_audioData;
    QScopedPointer<QBuffer> m_buffer;
    QScopedPointer<QAudioSink> m_audioSink;
};

#endif // TONEGENERATOR_H

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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QCloseEvent>
#include <QLabel>
#include <QMainWindow>
#include <QMenu>
#include <QPushButton>
#include <QSlider>
#include <QSpinBox>
#include <QSystemTrayIcon>
#include <QTimer>

class ToneGenerator;

class MainWindow : public QMainWindow {
    Q_OBJECT

  protected:
    void closeEvent(QCloseEvent *event) override;

  public:
    explicit MainWindow(int frequency, bool startMinimized = false,
                        QWidget *parent = nullptr);
    ~MainWindow();

  private slots:
    void toggleKeepAlive();
    void updateInterval(int seconds);
    void updateDuration(int ms);
    void updateVolume(int value);
    void updateFrequency(int Hz);

  private:
    void setupUi();
    void setupSystemTray();
    void toggleVisibility();
    void createConnections();
    void loadStyleSheet();

    ToneGenerator *m_toneGenerator;
    QTimer *m_intervalTimer;

    QPushButton *m_toggleButton;
    QSpinBox *m_intervalSpinBox;
    QSpinBox *m_durationSpinBox;
    QSlider *m_volumeSlider;
    QLabel *m_volumeLabel;
    QSlider *m_frequencySlider;
    QLabel *m_frequencyLabel;

    QSystemTrayIcon *m_trayIcon = nullptr;
    QMenu *m_trayMenu = nullptr;
    bool m_minimizeToTray = true;

    int m_frequency;
    bool m_isRunning = false;
    bool m_startMinimized = false;
};

#endif // MAINWINDOW_H

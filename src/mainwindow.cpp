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

#include "mainwindow.h"
#include "components.h"
#include "tonegenerator.h"
#include <QApplication>
#include <QFile>
#include <QStyle>
#include <QVBoxLayout>

MainWindow::MainWindow(int frequency, bool startMinimized, QWidget *parent)
    : QMainWindow(parent), m_toneGenerator(new ToneGenerator(this)),
      m_intervalTimer(new QTimer(this)), m_frequency(frequency),
      m_startMinimized(startMinimized) {
    m_toneGenerator->setFrequency(m_frequency);

    setupUi();
    createConnections();
    setupSystemTray();
    loadStyleSheet();

    setWindowTitle(APPLICATION_NAME);
    setFixedSize(350, 350);

    if (m_startMinimized)
        toggleKeepAlive();
}

MainWindow::~MainWindow() = default;

void MainWindow::loadStyleSheet() {
    QFile styleFile(":/styles/style.qss");
    if (styleFile.open(QFile::ReadOnly | QFile::Text)) {
        qApp->setStyleSheet(styleFile.readAll());
        styleFile.close();
    }
}

void MainWindow::setupUi() {
    auto *centralWidget = new QWidget(this);
    auto *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(12);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    auto *infoLabel = new QLabel(
        QString("Plays a %1 Hz tone to prevent speakers from sleeping.\n"
                "Most adults cannot hear frequencies above 15kHz.")
            .arg(m_frequency));
    infoLabel->setObjectName("infoLabel");
    infoLabel->setWordWrap(true);
    mainLayout->addWidget(infoLabel);

    mainLayout->addWidget(Components::createIntervalGroup(m_intervalSpinBox));
    mainLayout->addWidget(Components::createDurationGroup(m_durationSpinBox));
    mainLayout->addWidget(
        Components::createVolumeGroup(m_volumeSlider, m_volumeLabel));
    mainLayout->addWidget(Components::createFrequencyGroup(
        m_frequencySlider, m_frequencyLabel, m_frequency));

    m_toggleButton = Components::createToggleButton();
    mainLayout->addWidget(m_toggleButton);

    mainLayout->addStretch();
    setCentralWidget(centralWidget);
}

void MainWindow::setupSystemTray() {
    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        return;
    }

    m_trayIcon = new QSystemTrayIcon(this);
    m_trayIcon->setIcon(QIcon(":icons/icon.png"));
    m_trayIcon->setToolTip(APPLICATION_NAME);

    m_trayMenu = new QMenu(this);

    auto *showAction = m_trayMenu->addAction("Show/Hide");
    connect(showAction, &QAction::triggered, this,
            &MainWindow::toggleVisibility);

    auto *toggleAction = m_trayMenu->addAction("Toggle Keep-Alive");
    connect(toggleAction, &QAction::triggered, this,
            &MainWindow::toggleKeepAlive);

    m_trayMenu->addSeparator();

    auto *quitAction = m_trayMenu->addAction("Quit");
    connect(quitAction, &QAction::triggered, qApp, &QApplication::quit);

    m_trayIcon->setContextMenu(m_trayMenu);
    m_trayIcon->show();

    connect(m_trayIcon, &QSystemTrayIcon::activated, this,
            [this](QSystemTrayIcon::ActivationReason reason) {
                if (reason == QSystemTrayIcon::DoubleClick) {
                    toggleVisibility();
                }
            });
}

void MainWindow::toggleVisibility() {
    if (isVisible()) {
        hide();
    } else {
        showNormal();
        activateWindow();
        raise();
    }
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if (m_minimizeToTray && m_trayIcon && m_trayIcon->isVisible()) {
        hide();
        m_trayIcon->showMessage(
            APPLICATION_NAME,
            "Application minimzed to tray. Double click to restore.",
            QSystemTrayIcon::Information, 2000);
        event->ignore();
    } else {
        event->accept();
    }
}

void MainWindow::createConnections() {
    connect(m_toggleButton, &QPushButton::clicked, this,
            &MainWindow::toggleKeepAlive);
    connect(m_intervalSpinBox, &QSpinBox::valueChanged, this,
            &MainWindow::updateInterval);
    connect(m_durationSpinBox, &QSpinBox::valueChanged, this,
            &MainWindow::updateDuration);
    connect(m_volumeSlider, &QSlider::valueChanged, this,
            &MainWindow::updateVolume);
    connect(m_intervalTimer, &QTimer::timeout, m_toneGenerator,
            &ToneGenerator::playTone);

    connect(m_volumeSlider, &QSlider::valueChanged, this,
            [this](int v) { m_volumeLabel->setText(QString("%1%").arg(v)); });
    connect(m_frequencySlider, &QSlider::valueChanged, this,
            [this](int f) -> void {
                int hz = f * 100;
                m_frequencyLabel->setText(QString("%1Hz").arg(hz));
                updateFrequency(hz);
            });
}

void MainWindow::toggleKeepAlive() {
    m_isRunning = !m_isRunning;

    if (m_isRunning) {
        m_toneGenerator->playTone();
        m_intervalTimer->start(m_intervalSpinBox->value() * 1000);
        m_toggleButton->setText("Stop Keep-Alive");
        m_toggleButton->setProperty("running", true);
    } else {
        m_intervalTimer->stop();
        m_toggleButton->setText("Start Keep-Alive");
        m_toggleButton->setProperty("running", false);
    }

    m_toggleButton->style()->unpolish(m_toggleButton);
    m_toggleButton->style()->polish(m_toggleButton);
}

void MainWindow::updateInterval(int seconds) {
    if (m_isRunning) {
        m_intervalTimer->setInterval(seconds * 1000);
    }
}

void MainWindow::updateDuration(int ms) { m_toneGenerator->setDuration(ms); }

void MainWindow::updateVolume(int value) {
    m_toneGenerator->setVolume(value / 100.0f);
}

void MainWindow::updateFrequency(int hz) { m_toneGenerator->setFrequency(hz); }

#include "mainwindow.h"
#include "components.h"
#include "tonegenerator.h"
#include <QApplication>
#include <QFile>
#include <QStyle>
#include <QVBoxLayout>

MainWindow::MainWindow(int frequency, QWidget *parent)
    : QMainWindow(parent), m_toneGenerator(new ToneGenerator(this)),
      m_intervalTimer(new QTimer(this)), m_frequency(frequency) {
    m_toneGenerator->setFrequency(m_frequency);

    setupUi();
    createConnections();
    setupSystemTray();
    loadStyleSheet();

    setWindowTitle(APPLICATION_NAME);
    setFixedSize(350, 300);
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

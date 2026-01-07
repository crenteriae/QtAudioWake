#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QSlider>
#include <QSpinBox>
#include <QTimer>

class ToneGenerator;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(int frequency, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void toggleKeepAlive();
    void updateInterval(int seconds);
    void updateDuration(int ms);
    void updateVolume(int value);

private:
    void setupUi();
    void createConnections();
    void loadStyleSheet();

    ToneGenerator *m_toneGenerator;
    QTimer *m_intervalTimer;

    QPushButton *m_toggleButton;
    QSpinBox *m_intervalSpinBox;
    QSpinBox *m_durationSpinBox;
    QSlider *m_volumeSlider;
    QLabel *m_volumeLabel;

    int m_frequency;
    bool m_isRunning = false;
};

#endif // MAINWINDOW_H

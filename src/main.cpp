#include "mainwindow.h"
#include <QApplication>
#include <QCommandLineParser>

namespace {
const QString kFrequencyOptName = "frequency";
const QString kFrequencyOptShortcut = "f";
const QString kMinimizeOptName = "minimized";
const QString kMinimizeOptShortcut = "m";
constexpr int kMaxFrequency = 30000;
constexpr int kMinFrequency = 1000;

int parseFrequency(QCommandLineParser &parser) {
    bool ok;
    int frequency = parser.value(kFrequencyOptName).toInt(&ok);
    if (!ok || frequency < kMinFrequency || frequency > kMaxFrequency) {
        qWarning() << "Invalid frequency, using default:"
                   << DEFAULT_TONE_FREQUENCY;
        frequency = DEFAULT_TONE_FREQUENCY;
    }

    return frequency;
}

void addCmdArgs(QCommandLineParser &parser) {
    parser.addHelpOption();
    QCommandLineOption frequencyOption(
        QStringList() << kFrequencyOptShortcut << kFrequencyOptName,
        "Tone frequency in Hz (default: " +
            QString::number(DEFAULT_TONE_FREQUENCY) + ")",
        "hz", QString::number(DEFAULT_TONE_FREQUENCY));
    parser.addOption(frequencyOption);

    QCommandLineOption minimizeOption(
        QStringList() << kMinimizeOptShortcut << kMinimizeOptName,
        "Starts the application minimized to the system tray if set");
    parser.addOption(minimizeOption);
}

} // namespace

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(false);
    app.setApplicationName(APPLICATION_NAME);

    QCommandLineParser parser;
    parser.setApplicationDescription(
        "Plays a high-frequency tone to prevent speakers from sleeping");
    addCmdArgs(parser);
    parser.process(app);

    int frequency = parseFrequency(parser);
    bool startMinimized = parser.isSet(kMinimizeOptName);
    MainWindow window(frequency, startMinimized);

    if (!startMinimized)
        window.show();

    return app.exec();
}

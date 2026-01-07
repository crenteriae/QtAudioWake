#include <QApplication>
#include <QCommandLineParser>
#include "mainwindow.h"

const QString FREQUENCY_OPT_NAME = "frequency";
const QString FREQUENCY_OPT_SHORTCUT = "f";

int ParseFrequency(QCommandLineParser &parser) {
  bool ok;
  int frequency = parser.value(FREQUENCY_OPT_NAME).toInt(&ok);
  if (!ok || frequency < 1000 || frequency > 22000) {
      qWarning() << "Invalid frequency, using default:" << DEFAULT_TONE_FREQUENCY;
      frequency = DEFAULT_TONE_FREQUENCY;
  }

  return frequency;
}

void AddCmdArgs(QCommandLineParser &parser) {
  parser.addHelpOption();
  QCommandLineOption frequencyOption(
      QStringList() << FREQUENCY_OPT_SHORTCUT << FREQUENCY_OPT_NAME,
      "Tone frequency in Hz (default: " + QString::number(DEFAULT_TONE_FREQUENCY) + ")",
      "hz",
      QString::number(DEFAULT_TONE_FREQUENCY)
  );
  parser.addOption(frequencyOption);
}

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  app.setApplicationName("QtAudioWake");

  QCommandLineParser parser;
  parser.setApplicationDescription("Plays a high-frequency tone to prevent speakers from sleeping");
  AddCmdArgs(parser);
  parser.process(app);

  int frequency = ParseFrequency(parser);
  MainWindow window(frequency);
  window.show();

  return app.exec();
}

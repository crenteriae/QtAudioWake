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

#include "constants.h"
#include "mainwindow.h"
#include "settings.h"
#include <QApplication>
#include <QCommandLineParser>

namespace {
void addCmdArgs(QCommandLineParser &parser) {
    parser.addHelpOption();
    QCommandLineOption frequencyOption(
        QStringList() << kFrequencyOptShortcut << kFrequencyOptName,
        "Tone frequency in Hz (default: from settings, otherwise: " +
            QString::number(DEFAULT_TONE_FREQUENCY) + ")",
        "hz");
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

    auto parseIntOpt = [&](const QString name, int lo, int hi, int &target) {
        if (!parser.isSet(name))
            return;

        bool ok = false;
        int value = parser.value(name).toInt(&ok);
        if (ok && value >= lo && value <= hi)
            target = value;
        else
            qWarning() << "Invalid " << name << " keeping " << target;
    };

    auto cfg = AppSettings::loadStartupConfig();
    parseIntOpt(kFrequencyOptName, kMinFrequency, kMaxFrequency,
                cfg.frequencyHz);
    cfg.startMinimized =
        parser.isSet(kMinimizeOptName) ? true : cfg.startMinimized;
    MainWindow window(cfg);

    if (!cfg.startMinimized)
        window.show();

    return app.exec();
}

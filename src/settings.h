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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>

namespace AppSettings {

QSettings &getSettings();

void persistSettings(int frequency, int volumePercent, int interval,
                     int duration, bool startMinimized);

struct StartupConfig {
    int frequencyHz;
    int volumePercent;
    int intervalSec;
    int durationMs;
    bool startMinimized;
};

StartupConfig loadStartupConfig();

} // namespace AppSettings
#endif // !SETTINGS_H

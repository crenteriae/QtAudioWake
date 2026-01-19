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

#include "settings.h"
#include "constants.h"
#include <QSettings>
#include <QVariant>

namespace {
static int loadIntClamped(const char *key, int fallback, int lo, int hi) {
    auto &settings = AppSettings::getSettings();
    const QVariant value = settings.value(key, fallback);
    bool ok = false;
    int out = value.toInt(&ok);
    if (!ok)
        out = fallback;
    return std::clamp(out, lo, hi);
}
static bool loadBool(const char *key) {
    auto &settings = AppSettings::getSettings();
    const QVariant value = settings.value(key);
    return value.toBool();
}
} // namespace

namespace AppSettings {

QSettings &getSettings() {
    static QSettings s("crenteriae", APPLICATION_NAME);
    return s;
}

void persistSettings(int frequency, int volumePercent, int interval,
                     int duration, bool startMinimized) {
    auto &settings = getSettings();
    settings.setValue("frequency", frequency);
    settings.setValue("volume", volumePercent);
    settings.setValue("interval", interval);
    settings.setValue("duration", duration);
    settings.setValue("startMinimized", startMinimized);
}
StartupConfig loadStartupConfig() {
    StartupConfig cfg{};
    cfg.frequencyHz = loadIntClamped("frequency", DEFAULT_TONE_FREQUENCY,
                                     kMinFrequency, kMaxFrequency);
    cfg.volumePercent = loadIntClamped("volume", 30, 1, 100);
    cfg.intervalSec = loadIntClamped("interval", 30, 1, 300);
    cfg.durationMs = loadIntClamped("duration", 50, 10, 500);
    cfg.startMinimized = loadBool("startMinimized");

    return cfg;
}
} // namespace AppSettings

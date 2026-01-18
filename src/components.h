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

#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QSpinBox>

namespace Components {

QGroupBox *createIntervalGroup(QSpinBox *&outSpinBox);
QGroupBox *createDurationGroup(QSpinBox *&outSpinBox);
QGroupBox *createVolumeGroup(QSlider *&outSlider, QLabel *&outVolumeLabel);
QPushButton *createToggleButton();

} // namespace Components

#endif // COMPONENTS_H

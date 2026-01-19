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

#include "components.h"
#include "constants.h"
#include <QHBoxLayout>

namespace Components {

QGroupBox *createIntervalGroup(QSpinBox *&outSpinBox, int defaultInterval) {
    auto *group = new QGroupBox("Interval");
    auto *layout = new QHBoxLayout(group);
    layout->setContentsMargins(10, 5, 10, 5);
    layout->setAlignment(Qt::AlignVCenter);

    layout->addWidget(new QLabel("Play every:"));

    outSpinBox = new QSpinBox();
    outSpinBox->setRange(1, 300);
    outSpinBox->setValue(defaultInterval);
    outSpinBox->setSuffix(" sec");
    outSpinBox->setMinimumWidth(100);

    layout->addWidget(outSpinBox);
    layout->addStretch();

    return group;
}

QGroupBox *createDurationGroup(QSpinBox *&outSpinBox, int defaultDuration) {
    auto *group = new QGroupBox("Tone Duration");
    auto *layout = new QHBoxLayout(group);
    layout->setContentsMargins(10, 5, 10, 5);
    layout->setAlignment(Qt::AlignVCenter);

    layout->addWidget(new QLabel("Length:"));

    outSpinBox = new QSpinBox();
    outSpinBox->setRange(10, 500);
    outSpinBox->setValue(defaultDuration);
    outSpinBox->setSuffix(" ms");
    outSpinBox->setSingleStep(10);
    outSpinBox->setMinimumWidth(100);

    layout->addWidget(outSpinBox);
    layout->addStretch();

    return group;
}

QGroupBox *createVolumeGroup(QSlider *&outSlider, QLabel *&outVolumeLabel,
                             int defaultVolume) {
    auto *group = new QGroupBox("Volume");
    auto *layout = new QHBoxLayout(group);
    layout->setContentsMargins(10, 5, 10, 5);
    layout->setAlignment(Qt::AlignVCenter);

    outSlider = new QSlider(Qt::Horizontal);
    outSlider->setRange(1, 100);
    outSlider->setValue(defaultVolume);

    outVolumeLabel = new QLabel(QString("%1%").arg(defaultVolume));
    outVolumeLabel->setMinimumWidth(40);

    layout->addWidget(outSlider);
    layout->addWidget(outVolumeLabel);

    return group;
}

QGroupBox *createFrequencyGroup(QSlider *&outSlider, QLabel *&outFrequencyLabel,
                                int defaultFrequency) {
    auto *group = new QGroupBox("Frequency");
    auto *layout = new QHBoxLayout(group);
    layout->setContentsMargins(10, 5, 10, 5);
    layout->setAlignment(Qt::AlignVCenter);

    outSlider = new QSlider(Qt::Horizontal);
    outSlider->setRange(kMinFrequency / 100, kMaxFrequency / 100);
    outSlider->setValue(defaultFrequency / 100);

    outFrequencyLabel = new QLabel(QString::number(defaultFrequency) + "Hz");
    outFrequencyLabel->setMinimumWidth(40);

    layout->addWidget(outSlider);
    layout->addWidget(outFrequencyLabel);

    return group;
}

QPushButton *createToggleButton() {
    auto *button = new QPushButton("Start Keep-Alive");
    button->setMinimumHeight(40);
    return button;
}
} // namespace Components

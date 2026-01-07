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

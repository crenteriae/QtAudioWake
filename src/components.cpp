#include "components.h"
#include <QHBoxLayout>

namespace Components {

QGroupBox *createIntervalGroup(QSpinBox *&outSpinBox)
{
    auto *group = new QGroupBox("Interval");
    auto *layout = new QHBoxLayout(group);

    layout->addWidget(new QLabel("Play every:"));

    outSpinBox = new QSpinBox();
    outSpinBox->setRange(1, 300);
    outSpinBox->setValue(30);
    outSpinBox->setSuffix(" sec");
    outSpinBox->setMinimumWidth(100);

    layout->addWidget(outSpinBox);
    layout->addStretch();

    return group;
}

QGroupBox *createDurationGroup(QSpinBox *&outSpinBox)
{
    auto *group = new QGroupBox("Tone Duration");
    auto *layout = new QHBoxLayout(group);

    layout->addWidget(new QLabel("Length:"));

    outSpinBox = new QSpinBox();
    outSpinBox->setRange(10, 500);
    outSpinBox->setValue(50);
    outSpinBox->setSuffix(" ms");
    outSpinBox->setSingleStep(10);
    outSpinBox->setMinimumWidth(100);

    layout->addWidget(outSpinBox);
    layout->addStretch();

    return group;
}

QGroupBox *createVolumeGroup(QSlider *&outSlider, QLabel *&outVolumeLabel)
{
    auto *group = new QGroupBox("Volume");
    auto *layout = new QHBoxLayout(group);

    outSlider = new QSlider(Qt::Horizontal);
    outSlider->setRange(1, 100);
    outSlider->setValue(30);

    outVolumeLabel = new QLabel("30%");
    outVolumeLabel->setMinimumWidth(40);

    layout->addWidget(outSlider);
    layout->addWidget(outVolumeLabel);

    return group;
}

QPushButton *createToggleButton()
{
    auto *button = new QPushButton("Start Keep-Alive");
    button->setObjectName("toggleButton");
    button->setMinimumHeight(40);
    return button;
}
} // namespace Components

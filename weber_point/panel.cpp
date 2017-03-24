#include "panel.h"
#include <QGroupBox>
#include <QRadioButton>
#include <QPushButton>
#include <QVBoxLayout>

Panel::Panel(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(createSourceObstacleGroup());
    vbox->addWidget(new QPushButton(tr("Initialize"), this));
    vbox->addStretch(1);
    setLayout(vbox);
}

QGroupBox* Panel::createSourceObstacleGroup()
{
    QGroupBox *groupBox = new QGroupBox(tr("Input Setup"));

    QRadioButton *radio1 = new QRadioButton(tr("&Source Rectangle"));
    QRadioButton *radio2 = new QRadioButton(tr("Source Polygon"));
    QRadioButton *radio3 = new QRadioButton(tr("Obstacle Rectangle"));
    QRadioButton *radio4 = new QRadioButton(tr("Obstacle Polygon"));

    radio1->setChecked(true);

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(radio1);
    vbox->addWidget(radio2);
    vbox->addWidget(radio3);
    vbox->addWidget(radio4);
    vbox->addStretch(1);
    groupBox->setLayout(vbox);

    return groupBox;
}

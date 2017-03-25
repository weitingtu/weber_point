#include "panel.h"
#include <QGroupBox>
#include <QRadioButton>
#include <QPushButton>
#include <QVBoxLayout>

Panel::Panel(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(createSourceObstacleGroup());
    vbox->addWidget(new QPushButton(tr("&Clear"), this));
    vbox->addWidget(new QPushButton(tr("&Hexagonal"), this));
    vbox->addWidget(new QPushButton(tr("&Accumulation"), this));
    vbox->addWidget(new QPushButton(tr("&Decompose"), this));
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

    connect(radio1, SIGNAL(toggled(bool)), this, SLOT(_create_source_rect(bool)));
    connect(radio2, SIGNAL(toggled(bool)), this, SLOT(_create_source_poly(bool)));
    connect(radio3, SIGNAL(toggled(bool)), this, SLOT(_create_obs_rect(bool)));
    connect(radio4, SIGNAL(toggled(bool)), this, SLOT(_create_obs_poly(bool)));

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

void Panel::_create_source_rect( bool checked ) const
{
    if( checked )
    {
        emit mode_changed( MODE::CREATE_SOURCE_RECT );
    }
}

void Panel::_create_source_poly( bool checked ) const
{
    if( checked )
    {
        emit mode_changed( MODE::CREATE_SOURCE_POLY );
    }
}

void Panel::_create_obs_rect( bool checked ) const
{
    if( checked )
    {
        emit mode_changed( MODE::CREATE_OBS_RECT );
    }
}

void Panel::_create_obs_poly( bool checked ) const
{
    if( checked )
    {
        emit mode_changed( MODE::CREATE_OBS_POLY );
    }
}

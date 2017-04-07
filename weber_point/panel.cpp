#include "panel.h"
#include <QGroupBox>
#include <QRadioButton>
#include <QPushButton>
#include <QVBoxLayout>

Panel::Panel(QWidget *parent) : QWidget(parent),
    _clear(new QPushButton(tr("&Clear"), this)),
    _hex(new QPushButton(tr("&Hexagonal"), this)),
    _cdt(new QPushButton(tr("C&DT"), this)),
    _fermat_point(new QPushButton(tr("&Fermat Point"), this)),
    _wave_propagate(new QPushButton(tr("&Wave Propagate"), this)),
    _decompose(new QPushButton(tr("&Decompose"), this))
{
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(_createSourceObstacleGroup());
    vbox->addWidget(_clear);
    vbox->addWidget(_hex);
    vbox->addWidget(_cdt);
    vbox->addWidget(_fermat_point);
    vbox->addWidget(_wave_propagate);
    vbox->addWidget(_decompose);
    vbox->addStretch(1);
    setLayout(vbox);
}

QGroupBox* Panel::_createSourceObstacleGroup()
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

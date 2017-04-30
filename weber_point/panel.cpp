#include "panel.h"
#include <QGroupBox>
#include <QSpinBox>
#include <QRadioButton>
#include <QPushButton>
#include <QComboBox>
#include <QVBoxLayout>

Panel::Panel(QWidget *parent) : QWidget(parent),
    _font_size(new QSpinBox(this)),
    _clear(new QPushButton(tr("&Clear"), this)),
    _hex(new QPushButton(tr("&Hexagonal"), this)),
    _cdt(new QPushButton(tr("C&DT"), this)),
    _fermat_point(new QPushButton(tr("Center of &Gravity"), this)),
    _wave_propagate(new QPushButton(tr("&Wave Propagate"), this)),
    _decompose(new QPushButton(tr("&Decompose"), this)),
    _box(new QComboBox(this))
{
    _font_size->setValue(9);

    connect(_box, SIGNAL(activated(int)), this, SIGNAL(activated(int)));

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(_font_size);
    vbox->addWidget(_createSourceObstacleGroup());
    vbox->addWidget(_clear);
    vbox->addWidget(_hex);
    vbox->addWidget(_cdt);
    vbox->addWidget(_fermat_point);
    vbox->addWidget(_wave_propagate);
    vbox->addWidget(_decompose);
    vbox->addWidget(_box);
    vbox->addStretch(1);
    setLayout(vbox);

    clear();
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

void Panel::set_source_number(int s)
{
    clear();
    for(int i = 0; i < s; ++i)
    {
        _box->addItem(QString("Source %1").arg(QString::number(i)));
    }
    if(s > 0)
    {
        _box->addItem("Total");
    }
}

void Panel::clear()
{
    _box->clear();
    _box->addItem("None");
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

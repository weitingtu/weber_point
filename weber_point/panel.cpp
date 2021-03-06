#include "panel.h"
#include <QGroupBox>
#include <QSpinBox>
#include <QRadioButton>
#include <QPushButton>
#include <QComboBox>
#include <QVBoxLayout>
#include <QLineEdit>

Panel::Panel(QWidget *parent) : QWidget(parent),
    _font_size(new QSpinBox(this)),
    _clear(new QPushButton(tr("&Clear"), this)),
    _hex(new QPushButton(tr("&Hexagonal"), this)),
    _cdt(new QPushButton(tr("C&DT"), this)),
    _fermat_point(new QPushButton(tr("Center of &Gravity"), this)),
    _wave_propagate(new QPushButton(tr("&Wave Propagate"), this)),
    _decompose(new QPushButton(tr("&Decompose"), this)),
    _wp_box(new QComboBox(this)),
    _vg_box(new QComboBox(this)),
    _difference(new QComboBox(this)),
    _old_value(new QLineEdit(this)),
    _new_value(new QLineEdit(this)),
    _hex_secs(new QLabel("0", this)),
    _cdt_secs(new QLabel("0", this)),
    _fer_secs(new QLabel("0", this)),
    _wp_secs(new QLabel("0", this)),
    _dec_secs(new QLabel("0", this)),
    _total_secs(new QLabel("0", this))
{
    _font_size->setValue(9);

    connect(_wp_box, SIGNAL(activated(int)), this, SIGNAL(wp_activated(int)));
    connect(_vg_box, SIGNAL(activated(int)), this, SIGNAL(vg_activated(int)));

    _difference->addItem("1%",      0.01);
    _difference->addItem("0.1%",    0.001);
    _difference->addItem("0.01%",   0.0001);
    _difference->addItem("0.001%",  0.00001);
    _difference->addItem("0.0001%", 0.000001);
    _old_value->setReadOnly(true);
    _new_value->setReadOnly(true);

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(_font_size);
    vbox->addWidget(_createSourceObstacleGroup());
    vbox->addWidget(_clear);
    vbox->addWidget(_hex);
    vbox->addWidget(_hex_secs);
    vbox->addWidget(_cdt);
    vbox->addWidget(_cdt_secs);
    vbox->addWidget(_fermat_point);
    vbox->addWidget(_fer_secs);
    vbox->addWidget(_wave_propagate);
    vbox->addWidget(_wp_secs);
    vbox->addWidget(_decompose);
    vbox->addWidget(_dec_secs);
    vbox->addWidget(_total_secs);
    vbox->addWidget(_wp_box);
    vbox->addWidget(_vg_box);
    vbox->addWidget(_difference);
    vbox->addWidget(_old_value);
    vbox->addWidget(_new_value);
    vbox->addStretch(1);
    setLayout(vbox);

    clear();
}

QGroupBox* Panel::_createSourceObstacleGroup()
{
    QGroupBox *groupBox = new QGroupBox(tr("Input Setup"));

    QRadioButton *radio1 = new QRadioButton(tr("&Source Point"));
    QRadioButton *radio2 = new QRadioButton(tr("Source Rectangle"));
    QRadioButton *radio3 = new QRadioButton(tr("Source Polygon"));
    QRadioButton *radio4 = new QRadioButton(tr("Obstacle Rectangle"));
    QRadioButton *radio5 = new QRadioButton(tr("Obstacle Polygon"));

    connect(radio1, SIGNAL(toggled(bool)), this, SLOT(_create_source_point(bool)));
    connect(radio2, SIGNAL(toggled(bool)), this, SLOT(_create_source_rect(bool)));
    connect(radio3, SIGNAL(toggled(bool)), this, SLOT(_create_source_poly(bool)));
    connect(radio4, SIGNAL(toggled(bool)), this, SLOT(_create_obs_rect(bool)));
    connect(radio5, SIGNAL(toggled(bool)), this, SLOT(_create_obs_poly(bool)));

    radio1->setChecked(true);

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(radio1);
    vbox->addWidget(radio2);
    vbox->addWidget(radio3);
    vbox->addWidget(radio4);
    vbox->addWidget(radio5);
    vbox->addStretch(1);
    groupBox->setLayout(vbox);

    return groupBox;
}

void Panel::set_value(double o, double n)
{
    _old_value->setText(QString::number(o));
    _new_value->setText(QString::number(n));
}

void Panel::set_source_number(int s)
{
    _clear_weight();
    for(int i = 0; i < s; ++i)
    {
        _wp_box->addItem(QString("Source %1").arg(QString::number(i)));
        _vg_box->addItem(QString("Source %1").arg(QString::number(i)));
    }
    if(s > 0)
    {
        _wp_box->addItem("Total");
        _vg_box->addItem("Total");
    }
}

void Panel::clear()
{
    set_hex_secs(0);
    set_cdt_secs(0);
    set_fer_secs(0);
    set_wp_secs(0);
    set_dec_secs(0);
    set_total_secs(0);
    _clear_weight();
}

void Panel::_clear_weight()
{
    _wp_box->clear();
    _wp_box->addItem("None");
    _vg_box->clear();
    _vg_box->addItem("None");
    _old_value->clear();
    _new_value->clear();
}

void Panel::_create_source_point( bool checked ) const
{
    if( checked )
    {
        emit mode_changed( MODE::CREATE_SOURCE_POINT );
    }
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

#ifndef PANEL_H
#define PANEL_H

#include "enum.h"
#include <QWidget>
#include <QLabel>

class QGroupBox;
class QSpinBox;
class QPushButton;
class QComboBox;
class QLineEdit;

class Panel : public QWidget
{
    Q_OBJECT
public:
    explicit Panel(QWidget *parent = 0);

    QSpinBox*    get_font_size_spin_box() const { return _font_size; }
    QPushButton* get_clear_button() const { return _clear; }
    QPushButton* get_hex_button() const { return _hex; }
    QPushButton* get_cdt_button() const { return _cdt; }
    QPushButton* get_fermat_point_button() const { return _fermat_point; }
    QPushButton* get_wave_propagate_button() const { return _wave_propagate; }
    QPushButton* get_decompose_button() const { return _decompose; }
    QComboBox*   get_difference_button() const { return _difference; }
    void set_value(double o, double n);
    void set_source_number(int s);
    void clear();

    void set_hex_secs(int s) const { _hex_secs->setText(QString::number(s) + " ms"); }
    void set_cdt_secs(int s) const { _cdt_secs->setText(QString::number(s) + " ms"); }
    void set_fer_secs(int s) const { _fer_secs->setText(QString::number(s) + " ms"); }
    void set_wp_secs(int s) const  { _wp_secs->setText(QString::number(s) + " ms"); }
    void set_dec_secs(int s) const { _dec_secs->setText(QString::number(s) + " ms"); }
    void set_total_secs(int s) const { _total_secs->setText(QString::number(s) + " ms"); }

signals:
    void mode_changed(MODE m) const;
//    void wp_activated(int index) const;
    void vg_activated(int index) const;

public slots:
private slots:
    void _create_source_rect(bool checked) const;
    void _create_source_poly(bool checked) const;
    void _create_obs_rect(bool checked) const;
    void _create_obs_poly(bool checked) const;

private:
    void _clear_weight();

    QGroupBox* _createSourceObstacleGroup();

    QSpinBox*    _font_size;
    QPushButton* _clear;
    QPushButton* _hex;
    QPushButton* _cdt;
    QPushButton* _fermat_point;
    QPushButton* _wave_propagate;
    QPushButton* _decompose;
//    QComboBox*   _wp_box;
    QComboBox*   _vg_box;
    QComboBox*   _difference;
    QLineEdit*   _old_value;
    QLineEdit*   _new_value;
    QLabel*      _hex_secs;
    QLabel*      _cdt_secs;
    QLabel*      _fer_secs;
    QLabel*      _wp_secs;
    QLabel*      _dec_secs;
    QLabel*      _total_secs;
};

#endif // PANEL_H

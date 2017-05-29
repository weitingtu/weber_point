#ifndef PANEL_H
#define PANEL_H

#include "enum.h"
#include <QWidget>

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

signals:
    void mode_changed(MODE m) const;
    void activated(int index) const;

public slots:
private slots:
    void _create_source_rect(bool checked) const;
    void _create_source_poly(bool checked) const;
    void _create_obs_rect(bool checked) const;
    void _create_obs_poly(bool checked) const;

private:
    QGroupBox* _createSourceObstacleGroup();

    QSpinBox*    _font_size;
    QPushButton* _clear;
    QPushButton* _hex;
    QPushButton* _cdt;
    QPushButton* _fermat_point;
    QPushButton* _wave_propagate;
    QPushButton* _decompose;
    QComboBox*   _box;
    QComboBox*   _difference;
    QLineEdit*   _old_value;
    QLineEdit*   _new_value;
};

#endif // PANEL_H

#ifndef PANEL_H
#define PANEL_H

#include "enum.h"
#include <QWidget>

class QGroupBox;
class QPushButton;

class Panel : public QWidget
{
    Q_OBJECT
public:
    explicit Panel(QWidget *parent = 0);

    QPushButton* get_clear_button() const { return _clear; }
    QPushButton* get_hex_button() const { return _hex; }
    QPushButton* get_cdt_button() const { return _cdt; }
    QPushButton* get_accumulation_button() const { return _accumulation; }
    QPushButton* get_decompose_button() const { return _decompose; }

signals:
    void mode_changed(MODE m) const;

public slots:
private slots:
    void _create_source_rect(bool checked) const;
    void _create_source_poly(bool checked) const;
    void _create_obs_rect(bool checked) const;
    void _create_obs_poly(bool checked) const;

private:
    QGroupBox* _createSourceObstacleGroup();

    QPushButton* _clear;
    QPushButton* _hex;
    QPushButton* _cdt;
    QPushButton* _accumulation;
    QPushButton* _decompose;
};

#endif // PANEL_H

#ifndef PANEL_H
#define PANEL_H

#include "enum.h"
#include <QWidget>

class QGroupBox;

class Panel : public QWidget
{
    Q_OBJECT
public:
    explicit Panel(QWidget *parent = 0);

signals:
    void mode_changed(MODE m) const;

public slots:
private slots:
    void _create_source_rect(bool checked) const;
    void _create_source_poly(bool checked) const;
    void _create_obs_rect(bool checked) const;
    void _create_obs_poly(bool checked) const;

private:
    QGroupBox* createSourceObstacleGroup();
};

#endif // PANEL_H

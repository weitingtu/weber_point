#ifndef PANEL_H
#define PANEL_H

#include <QWidget>

class QGroupBox;

class Panel : public QWidget
{
    Q_OBJECT
public:
    explicit Panel(QWidget *parent = 0);

signals:

public slots:
private:
    QGroupBox* createSourceObstacleGroup();
};

#endif // PANEL_H

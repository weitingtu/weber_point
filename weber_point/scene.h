#ifndef SCENE_H
#define SCENE_H

#include "enum.h"
#include <QGraphicsScene>
#include <QPointF>
#include <QLineF>
#include <QVector>

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Scene(QObject*parent = 0);

    void stop_input() { _accept_input = false; }

    void clear_all();

    void add_text(const QPointF& p, const QString& text);
    void clear_texts();
    void adjust_texts(int size);

    void add_vg_lines(const QVector<QLineF>& lines);
    void add_vg_pathes(const QVector<QLineF>& lines);
    void clear_vg_lines();

    void add_hex_point(double x, double y, double rad);
    void clear_hex_points();

    void add_cdt_lines(const QVector<QLineF>& lines);
    void clear_cdt_lines();

    void add_fermat_point(double x, double y, double rad);
    void add_fermat_line(const QLineF& line);
    void clear_fermat();

signals:

public slots:
    void set_mode(MODE m);
    void initialize();
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;

private:
    MODE _mode;
    bool _accept_input;
    QVector<QPointF>               _points;
    QVector<QGraphicsTextItem*>    _texts;
    QVector<QGraphicsLineItem*>    _vg_lines;
    QVector<QGraphicsEllipseItem*> _hex_points;
    QVector<QGraphicsLineItem*>    _cdt_lines;
    QVector<QGraphicsEllipseItem*> _fermat_points;
    QVector<QGraphicsLineItem*>    _fermat_lines;
};

#endif // SCENE_H

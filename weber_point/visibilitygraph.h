#ifndef VISIBILITYGRAPH_H
#define VISIBILITYGRAPH_H

#include <QVector>
#include <QPointF>
#include <QLineF>
#include <QPolygonF>

class VisibilityGraph
{
public:
    VisibilityGraph():
        _lines()
        {}

    void create(const QVector<QPointF>& sources, const QVector<QPolygonF>& obs, const QVector<QPointF>& target);
    const QVector<QLineF>& get_lines() const { return _lines; }

private:
    bool _is_intersected(const QLineF& l, const QPolygonF& poly) const;
    bool _is_blocked(const QLineF& l, const QVector<QPolygonF>& polies) const;
    void _dijkstra(const QVector<QVector<double> >& w, int source, QVector<double>& d);

    QVector<QLineF> _lines;
};

#endif // VISIBILITYGRAPH_H
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
        _points(),
        _weights(),
        _lines(),
        _pathes()
        {}

    void create(const QVector<QPointF>& sources, const QVector<QPolygonF>& obs, const QVector<QPointF>& target);
//    void create2(const QVector<QPointF>& sources,
//                 const QVector<QPolygonF> &source_polies, const QVector<QPolygonF>& obs, const QVector<QPointF>& target);
    const QVector<QPointF>& get_points() const { return _points; }
    const QVector<QVector<double>> get_weights() const { return _weights; }
    const QVector<QLineF>& get_lines() const { return _lines; }
    const QVector<QLineF>& get_pathes() const { return _pathes; }

private:
    bool _is_intersected(const QLineF& l, const QPolygonF& poly) const;
    bool _is_blocked(const QLineF& l, const QVector<QPolygonF>& polies) const;
    void _dijkstra(const QVector<QVector<double> >& w, int source, QVector<double>& d, QVector<int> &parent);

    QVector<QPointF> _points;
    QVector<QVector<double>> _weights;
    QVector<QLineF> _lines;
    QVector<QLineF> _pathes;
};

#endif // VISIBILITYGRAPH_H

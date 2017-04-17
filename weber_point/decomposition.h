#ifndef DECOMPOSITION_H
#define DECOMPOSITION_H

#include "poly.h"
#include <QVector>
#include <QPointF>

class QLineF;

class Decomposition
{
public:
    Decomposition(const Poly& p, const QVector<QPointF>& n, QVector<Poly>& graph, int idx) :
        _poly(p),
        _neighbors(n),
        _graph(graph),
        _idx(idx),
        _centers() {}

    void decompose();

    const QVector<QPointF>& get_centers() const { return _centers; }

private:
//    QPointF _get_intersection_point(const Poly& _poly, const QPointF &point) const;
    int _get_neighbor_poly(const QPointF &point, const QLineF& common_line, const QVector<Poly>& graph, const QVector<int>& neighbors) const;

    Poly             _poly;
    QVector<QPointF> _neighbors;
    QVector<Poly>&   _graph;
    int              _idx;
    QVector<QPointF> _centers;
};

#endif // DECOMPOSITION_H

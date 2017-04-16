#ifndef DECOMPOSITION_H
#define DECOMPOSITION_H

#include "poly.h"
#include <QVector>
#include <QPointF>

class Decomposition
{
public:
    Decomposition(const Poly& p, const QVector<QPointF>& n) : _poly(p), _neighbors(n), _centers() {}

    void decompose();

    const QVector<QPointF>& get_centers() const { return _centers; }

private:
    QPointF _get_intersection_point(const Poly& _poly, const QPointF &point) const;

    Poly             _poly;
    QVector<QPointF> _neighbors;
    QVector<QPointF> _centers;
};

#endif // DECOMPOSITION_H

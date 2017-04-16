#include "decomposition.h"
#include <QLineF>

QPointF Decomposition::_get_intersection_point(const Poly& poly, const QPointF& point) const
{
    QLineF line1(poly.center, point);
    for(int i = 0; i < poly.points.size() - 1; ++i)
    {
        QLineF line2(poly.points[i], poly.points[i+1]);
        QPointF p;
        if(QLineF::BoundedIntersection == line1.intersect(line2, &p))
        {
            return p;
        }
    }

    QLineF line2(poly.points.last(), poly.points.front());
    QPointF p;
    if(QLineF::BoundedIntersection == line1.intersect(line2, &p))
    {
        return p;
    }
    return QPointF();
}

void Decomposition::decompose()
{
    _centers.clear();

    for(int i = 0; i < _neighbors.size(); ++i)
    {
        QPointF p = _get_intersection_point(_poly, _neighbors[i]);
        double x = (p.x() - _poly.center.x()) / 3 + _poly.center.x();
        double y = (p.y() - _poly.center.y()) / 3 + _poly.center.y();
        _centers.push_back(QPointF(x, y));
    }
}

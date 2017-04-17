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

//    for(int i = 0; i < _neighbors.size(); ++i)
//    {
//        QPointF p = _get_intersection_point(_poly, _neighbors[i]);
//        double x = (p.x() - _poly.center.x()) / 3 * 2 + _poly.center.x();
//        double y = (p.y() - _poly.center.y()) / 3 * 2 + _poly.center.y();
//        _centers.push_back(QPointF(x, y));
//    }
    for(int i = 0; i < _poly.points.size(); ++i)
    {
        Poly p;
        p.points.push_back(_poly.center);
        p.points.push_back(_poly.points[i]);
        if(i == _poly.points.size() - 1)
        {
            p.points.push_back(_poly.points[0]);
        }
        else
        {
            p.points.push_back(_poly.points[i + 1]);
        }
        p.center = QPointF((p.points[0].x() + p.points[1].x() + p.points[2].x()) / 3, (p.points[0].y() + p.points[1].y() + p.points[2].y()) / 3);
        _centers.push_back(p.center);
    }
}

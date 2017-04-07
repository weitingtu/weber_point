#ifndef POLY_H
#define POLY_H

#include <QPointF>
#include <QVector>

class Poly
{
public:
    Poly() : center(), points(), indices(), neighbors()
    {}
    QPointF          center;
    QVector<QPointF> points;
    QVector<int>     indices;
    QVector<int>     neighbors;
};


#endif // POLY_H

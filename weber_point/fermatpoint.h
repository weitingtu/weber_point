#ifndef FERMATPOINT_H
#define FERMATPOINT_H

#include <QPointF>
#include <triangle.h>

class FermatPoint
{
public:
    static QPointF CalcFermatPoint(const QPointF& a, const QPointF& b, const QPointF& c);
    static QPointF CalcFermatPoint(const Triangle& t);
};

#endif // FERMATPOINT_H

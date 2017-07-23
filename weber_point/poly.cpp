#include "poly.h"
#include <QDebug>

bool Poly::is_neighbor(int idx) const
{
    for(int i = 0; i < neighbors.size(); ++i)
    {
        if(idx == neighbors[i])
        {
            return true;
        }
    }
    return false;
}

void Poly::clear()
{
    center = QPointF(0, 0);
    points.clear();
    indices.clear();
    neighbors.clear();
}

void Poly::print() const
{
    qDebug() << center;
    qDebug() << points;
    qDebug() << indices;
    qDebug() << neighbors;
}

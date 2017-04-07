#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <QPointF>

class Triangle
{
public:
    Triangle();
    QPointF points[3];
    int indices[3];
    int neighbors[3];
};

#endif // TRIANGLE_H

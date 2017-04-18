#ifndef DECOMPOSITION_H
#define DECOMPOSITION_H

#include "poly.h"
#include <QVector>
#include <QPointF>

class QLineF;

class Decomposition
{
public:
    static void decompose(QVector<Poly> &graph, int idx);

private:
    static int _get_neighbor_poly(const QPointF &point, const QLineF& common_line, const QVector<Poly>& graph, const QVector<int>& neighbors);
};

#endif // DECOMPOSITION_H

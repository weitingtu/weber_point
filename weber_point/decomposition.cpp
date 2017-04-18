#include "decomposition.h"
#include <QLineF>

int Decomposition::_get_neighbor_poly(const QPointF& point, const QLineF& common_line, const QVector<Poly>& graph, const QVector<int>& neighbors)
{
    for(int i = 0; i < neighbors.size(); ++i)
    {
        const Poly& poly = graph[neighbors[i]];
        QLineF line(point, poly.center);
        QPointF p;
        if(QLineF::BoundedIntersection == common_line.intersect(line, &p))
        {
            return neighbors[i];
        }
    }
    return -1;
}

void Decomposition::decompose(QVector<Poly>& graph, int idx)
{
    const Poly& poly = graph[idx];

    int poly_idx1 = graph.size();
    int poly_idx2 = graph.size() + 1;
    int poly_idx3 = graph.size() + 2;

    // decompose into 3 triangle poly
    for(int i = 0; i < poly.points.size(); ++i)
    {
        Poly p;
        QPointF p1 = poly.center;
        QPointF p2 = poly.points[i];
        QPointF p3 = i == (poly.points.size() - 1) ? poly.points[0] : poly.points[i + 1];
        p.points.push_back(p1);
        p.points.push_back(p2);
        p.points.push_back(p3);
        p.center = QPointF((p.points[0].x() + p.points[1].x() + p.points[2].x()) / 3, (p.points[0].y() + p.points[1].y() + p.points[2].y()) / 3);
        int neighbor_idx = _get_neighbor_poly(p1, QLineF(p2, p3), graph, poly.neighbors);
        if(0 <= neighbor_idx)
        {
            p.neighbors.push_back(neighbor_idx);
            int poly_idx = graph.size();
            Poly& neighbor_poly = graph[neighbor_idx];
            for(int j = 0; j < neighbor_poly.neighbors.size(); ++j)
            {
                if(neighbor_poly.neighbors[j] == idx)
                {
                    neighbor_poly.neighbors[j] = poly_idx;
                }
            }
        }
        graph.push_back(p);
    }

    graph[poly_idx1].neighbors.push_back(poly_idx2);
    graph[poly_idx1].neighbors.push_back(poly_idx3);
    graph[poly_idx2].neighbors.push_back(poly_idx1);
    graph[poly_idx2].neighbors.push_back(poly_idx3);
    graph[poly_idx3].neighbors.push_back(poly_idx1);
    graph[poly_idx3].neighbors.push_back(poly_idx2);

    graph[idx].neighbors.clear();
}

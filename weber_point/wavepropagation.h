#ifndef WAVEPROPAGATION_H
#define WAVEPROPAGATION_H

#include "poly.h"

struct Node
{
    Node(int i, double w) : idx(i), weight(w) {}
    int idx;
    double weight;
};

class WavePropagation
{
public:
    WavePropagation(const QVector<Poly>& g, int s);

    void propagate();

    const Poly& get_min_poly() const { return _graph[_min_idx]; }

private:

    double _distance(const QPointF& p1, const QPointF& p2 ) const;
    void bfs(const QVector<Poly>& graph, int source_idx, QVector<double>& weight) const;

    QVector<Poly> _graph;
    int           _source_idx;
    int           _min_idx;
};

#endif // WAVEPROPAGATION_H

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

    const QPointF& get_min_point() const { return _min_point; }

private:

    double _distance(const QPointF& p1, const QPointF& p2 ) const;
    void bfs(const QVector<Poly>& graph, int source_idx, QVector<double>& weight) const;

    QVector<Poly> _graph;
    int           _source_idx;
    QPointF       _min_point;
};

#endif // WAVEPROPAGATION_H

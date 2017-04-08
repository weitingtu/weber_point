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

private:

    double _distance(const QPointF& p1, const QPointF& p2 ) const;
    void _bfs(const QVector<Poly>& graph, int source_idx, QVector<double>& weight) const;
    void _bfs();

    QVector<Poly> _graph;
    int           _source_idx;
};

#endif // WAVEPROPAGATION_H

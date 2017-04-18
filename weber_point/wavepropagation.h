#ifndef WAVEPROPAGATION_H
#define WAVEPROPAGATION_H

#include "poly.h"

class WavePropagation
{
public:
    WavePropagation();

    void propagate(const QVector<Poly>& g, const QVector<Poly> &sg );
    void clear();

    int get_min_poly_idx() const { return _min_poly_idx; }

    const QVector<QVector<double> >& get_weights() const { return _weights; }
    const QVector<double>&           get_total_weight() const { return _total_weight; }

private:
    double _distance(const QPointF& p1, const QPointF& p2 ) const;
    void _bfs(const QVector<Poly>& graph, int source_idx, QVector<double>& weight) const;

    int                       _min_poly_idx;
    QVector<QVector<double> > _weights;
    QVector<double>           _total_weight;
};

#endif // WAVEPROPAGATION_H

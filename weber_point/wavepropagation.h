#ifndef WAVEPROPAGATION_H
#define WAVEPROPAGATION_H

#include "poly.h"

class WavePropagation
{
public:
    static WavePropagation& get_inst()
    {
        static WavePropagation inst;
        return inst;
    }

    void propagate(const QVector<Poly>& g, int s);
    void clear();

    const Poly& get_min_poly() const { return _graph[_min_idx]; }

    const QVector<QVector<double> >& get_weights() const { return _weights; }
    const QVector<double>&           get_total_weight() const { return _total_weight; }

private:
    WavePropagation();

    double _distance(const QPointF& p1, const QPointF& p2 ) const;
    void bfs(const QVector<Poly>& graph, int source_idx, QVector<double>& weight) const;

    QVector<Poly> _graph;
    int           _source_idx;
    int           _min_idx;
    QVector<QVector<double> > _weights;
    QVector<double>           _total_weight;
};

inline WavePropagation& get_wave_propagate()
{
    return WavePropagation::get_inst();
}

#endif // WAVEPROPAGATION_H

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

    void propagate(const QVector<Poly>& g, const QVector<Poly> &sg );
    void clear();

    const Poly& get_min_poly() const { return _min_poly; }

    const QVector<QVector<double> >& get_weights() const { return _weights; }
    const QVector<double>&           get_total_weight() const { return _total_weight; }

private:
    WavePropagation();

    double _distance(const QPointF& p1, const QPointF& p2 ) const;
    void bfs(const QVector<Poly>& graph, int source_idx, QVector<double>& weight) const;

    Poly                      _min_poly;
    QVector<QVector<double> > _weights;
    QVector<double>           _total_weight;
};

inline WavePropagation& get_wave_propagate()
{
    return WavePropagation::get_inst();
}

#endif // WAVEPROPAGATION_H

#include "wavepropagation.h"
#include <QQueue>

WavePropagation::WavePropagation(const QVector<Poly>& g, int s):
    _graph(g),
    _source_idx(s),
    _min_idx(-1),
    _weights(),
    _total_weight()
{
}

double WavePropagation::_distance(const QPointF& p1, const QPointF& p2 ) const
{
    double x = p1.x() - p2.x();
    double y = p1.y() - p2.y();

    return std::sqrt(x * x + y * y);
}

void WavePropagation::bfs(const QVector<Poly>& graph, int source_idx, QVector<double>& weight) const
{
    QQueue<int>     queue;
    QVector<bool>   popped(graph.size(), false);
    QVector<bool>   pushed(graph.size(), false);
    weight.clear();
    weight.resize(graph.size());
    weight.fill(0.0);

    pushed[source_idx] = true;
    weight[source_idx]  = 0.0;
    queue.push_back(source_idx);

    while(!queue.empty())
    {
        int s = queue.front();
        popped[s] = true;

        const Poly& p = graph[s];
        for(int i = 0; i < p.neighbors.size(); ++i)
        {
            int idx = p.neighbors[i];
            double w = _distance(p.center, graph[idx].center) + weight[s];
            if(pushed[idx])
            {
                if(!popped[idx] && weight[idx] > w)
                {
                    weight[idx] = w;
                }
                continue;
            }
            pushed[idx] = true;
            weight[idx] = w;
            queue.push_back(idx);
        }

        queue.pop_front();
    }
}

void WavePropagation::propagate()
{
    QVector<QVector<double> > weights(_graph.size() - _source_idx);
//    _weights.clear();
//    _weights.resize(_graph.size() - _source_idx);
//    QVector<QVector<double> >& weights = _weights;
    int idx = 0;
    for(int i = _source_idx; i < _graph.size(); ++i, ++idx)
    {
        bfs(_graph, i, weights[idx]);
    }

    QVector<double> total_weight( _graph.size(), 0.0);
//    _total_weight.clear();
//    _total_weight.resize( _graph.size());
//    _total_weight.fill(0.0);
//    QVector<double>& total_weight = _total_weight;
    for(int i = 0; i < weights.size(); ++i)
    {
        for(int j = 0; j < _graph.size(); ++j)
        {
            total_weight[j] += weights[i][j];
        }
    }

    int point_idx = -1;
    double min_weight = std::numeric_limits<double>::max();
    for(int i = 0; i < _source_idx; ++i)
    {
        if(total_weight[i] < min_weight)
        {
            point_idx = i;
            min_weight = total_weight[i];
        }
    }
    _min_idx = point_idx;
}

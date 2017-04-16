#include "wavepropagation.h"
#include <QQueue>

WavePropagation::WavePropagation():
    _min_poly_idx(-1),
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

void WavePropagation::propagate(const QVector<Poly>& g, const QVector<Poly>& sg)
{
    QVector<Poly> graph = g + sg;
    int source_idx = g.size();

    QVector<QVector<double> >& weights = _weights;
    weights.clear();
    weights.resize(sg.size());

    int idx = 0;
    for(int i = source_idx; i < graph.size(); ++i, ++idx)
    {
        bfs(graph, i, weights[idx]);
    }

    QVector<double>& total_weight = _total_weight;
    total_weight.clear();
    total_weight.resize( graph.size());
    total_weight.fill(0.0);

    for(int i = 0; i < weights.size(); ++i)
    {
        for(int j = 0; j < graph.size(); ++j)
        {
            total_weight[j] += weights[i][j] *weights[i][j];
        }
    }

    int point_idx = -1;
    double min_weight = std::numeric_limits<double>::max();
    for(int i = 0; i < source_idx; ++i)
    {
        if(total_weight[i] < min_weight)
        {
            point_idx = i;
            min_weight = total_weight[i];
        }
    }

    for(int i = 0; i < total_weight.size(); ++i)
    {
        total_weight[i] = sqrt(total_weight[i]);
    }
    _min_poly_idx = point_idx;
}

void WavePropagation::clear()
{
    _min_poly_idx = -1;
    _weights.clear();
    _total_weight.clear();
}

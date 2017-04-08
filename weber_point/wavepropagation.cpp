#include "wavepropagation.h"
#include <QQueue>

WavePropagation::WavePropagation(const QVector<Poly>& g, int s):
    _graph(g),
    _source_idx(s),
    _min_idx(-1)
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
    QQueue<Node>    queue;
    QVector<bool>   visited(graph.size(), false);
    weight.clear();
    weight.resize(graph.size());
    weight.fill(0.0);

    visited[source_idx] = true;
    queue.push_back(Node(source_idx, 0));

    while(!queue.empty())
    {
        const Node& s = queue.front();
        weight[s.idx] = s.weight;

        const Poly& p = graph[s.idx];
        for(int i = 0; i < p.neighbors.size(); ++i)
        {
            int idx = p.neighbors[i];
            if(visited[idx])
            {
                continue;
            }
            visited[idx] = true;
            double w = _distance(p.center, graph[idx].center) + weight[s.idx];
            queue.push_back(Node(idx, w));
        }

        queue.pop_front();
    }
}

void WavePropagation::propagate()
{
    QVector<QVector<double> > weights(_graph.size() - _source_idx);
    int idx = 0;
    for(int i = _source_idx; i < _graph.size(); ++i, ++idx)
    {
        bfs(_graph, i, weights[idx]);
    }

    QVector<double> total_weight( _source_idx, 0.0);
    for(int i = 0; i < weights.size(); ++i)
    {
        for(int j = 0; j < _source_idx; ++j)
        {
            total_weight[j] += weights[i][j];
        }
    }

    int point_idx = -1;
    double min_weight = std::numeric_limits<double>::max();
    for(int i = 0; i < total_weight.size(); ++i)
    {
        if(total_weight[i] < min_weight)
        {
            point_idx = i;
            min_weight = total_weight[i];
        }
    }
    _min_idx = point_idx;
}

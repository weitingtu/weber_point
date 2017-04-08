#include "wavepropagation.h"
#include <QQueue>

WavePropagation::WavePropagation(const QVector<Poly>& g, int s):
    _graph(g),
    _source_idx(s)
{
}

double WavePropagation::_distance(const QPointF& p1, const QPointF& p2 ) const
{
    double x = p1.x() - p2.x();
    double y = p1.y() - p2.y();

    return std::sqrt(x * x + y * y);
}

void WavePropagation::_bfs(const QVector<Poly>& graph, int source_idx, QVector<double>& weight) const
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
            double w = _distance(p.center, graph[idx].center);
            queue.push_back(Node(idx, w));
        }

        queue.pop_front();
    }
}

void WavePropagation::_bfs()
{
    for(int i = _source_idx; i < _graph.size(); ++i)
    {
        QVector<double> weight;
        _bfs(_graph, i, weight);
    }
}

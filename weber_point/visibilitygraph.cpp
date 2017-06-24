#include "visibilitygraph.h"
#include <QDebug>

bool VisibilityGraph::_is_intersected(const QLineF& l, const QPolygonF& poly) const
{
    for(int i = 0; i < poly.size() - 1; ++i)
    {
        QPointF p;
        if(QLineF::BoundedIntersection != l.intersect(QLineF(poly[i], poly[i+1]), &p))
        {
            continue;
        }
        if(p == poly[i])
        {
            continue;
        }
        if(p == poly[i+1])
        {
            continue;
        }
        return true;
    }
    return false;
}

bool VisibilityGraph::_is_blocked(const QLineF& l, const QVector<QPolygonF>& polies) const
{
    for(const QPolygonF& p : polies)
    {
        if(_is_intersected(l, p))
        {
            return true;
        }
    }
    return false;
}

void VisibilityGraph::_dijkstra(const QVector<QVector<double> >& w, int source, QVector<double>& d, QVector<int>& parent)
{
    QVector<bool> visited(w.size(), false);
    parent = QVector<int>(w.size(), std::numeric_limits<int>::max());
    d = QVector<double>(w.size(), std::numeric_limits<double>::max());
    d[source] = 0;
    parent[source] = source;

    for(int k = 0; k < w.size(); ++k)
    {
        int a = -1;
        int b = -1;
        double min = std::numeric_limits<double>::max();
        for(int i = 0; i < w.size(); ++i)
        {
            if(!visited[i] && d[i] < min)
            {
                a = i;
                min = d[i];
            }
        }

        if(a == -1)
        {
            break;
        }

        visited[a] = true;

        for(b = 0; b <w.size(); ++b)
        {
            if(!visited[b] && d[a] + w[a][b] < d[b])
            {
                d[b] = d[a] + w[a][b];
                parent[b] = a;
            }
        }
    }
}

void VisibilityGraph::create( const QVector<QPointF>& sources, const QVector<QPolygonF>& obs, const QVector<QPointF>& targets)
{
    QVector<QPointF> points = sources;
    int label = 0;
    QVector<int> labels(points.size(), label);

    for(const QPolygonF& p : obs)
    {
        ++label;
        for(int i = 0; i <p.size() - 1; ++i)
        {
            points.push_back(p[i]);
            labels.push_back(label);
        }
    }
    ++label;
    for(int i = 0; i < targets.size(); ++i)
    {
        points.push_back(targets[i]);
        labels.push_back(label);
    }

    QVector<QVector<double>> w(points.size(), QVector<double>(points.size(), std::numeric_limits<double>::max()));
    QVector<QVector<bool>> edges = QVector<QVector<bool> >(points.size(), QVector<bool>(points.size(), false));

    for(int i = 0; i < points.size(); ++i)
    {
        for(int j = i + 1; j <points.size(); ++j)
        {
            if(labels[i] == labels[j])
            {
                continue;
            }
            QLineF l(points[i], points[j]);
            if(!_is_blocked(l, obs))
            {
                edges[i][j] = true;
                edges[j][i] = true;
                w[i][j] = l.length();
                w[j][i] = l.length();
                _lines.push_back(l);
            }
        }
    }

    int start_idx = sources.size();
    for(int i = sources.size(); i < labels.size() - 1; ++i)
    {
        int idx1 = i;
        int idx2 = i+1;
        if(labels[idx1] != labels[idx2])
        {
            idx2 = start_idx;
            start_idx = i + 1;
        }
        edges[idx1][idx2] = true;
        edges[idx2][idx1] = true;
        QLineF l(points[idx1], points[idx2]);
        w[idx1][idx2] = l.length();
        w[idx2][idx1] = l.length();
        _lines.push_back(l);
    }

    for(int i = 0; i < sources.size(); ++i)
    {
        QVector<double> d;
        QVector<int> parent;
        _dijkstra( w, i, d, parent);
        for(int j = 1; j <= targets.size(); ++j)
        {
            int t = w.size() - j;
            int p = parent[t];
            while(p != t)
            {
                _pathes.push_back(QLineF(points[p], points[t]));
                t = p;
                p = parent[t];
            }
        }
    }
}

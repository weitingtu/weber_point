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

void VisibilityGraph::_dijkstra(const QVector<QPointF>& points, const QVector<QVector<double> >& w, int source, QVector<double>& d)
{
    QVector<bool>   visited(points.size(), false);
    QVector<int>    parent(points.size(), std::numeric_limits<int>::max());
    d = QVector<double>(points.size(), std::numeric_limits<double>::max());
    d[source] = 0;
    parent[source] = source;

    for(int k = 0; k < points.size(); ++k)
    {
        int a = -1;
        int b = -1;
        double min = std::numeric_limits<double>::max();
        for(int i = 0; i < points.size(); ++i)
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

        for(b = 0; b <points.size(); ++b)
        {
            if(!visited[b] && d[a] + w[a][b] < d[b])
            {
                d[b] = d[a] + w[a][b];
                parent[b] = a;
            }
        }
    }
}

void VisibilityGraph::create( const QVector<QPointF>& sources, const QVector<QPolygonF>& obs, const QPointF& target)
{
    _points = sources;
    int label = 0;
    QVector<int> labels(_points.size(), label);

    for(const QPolygonF& p : obs)
    {
        ++label;
        for(int i = 0; i <p.size() - 1; ++i)
        {
            _points.push_back(p[i]);
            labels.push_back(label);
        }
    }
    _points.push_back(target);
    labels.push_back(++label);

    QVector<QVector<double>> w(_points.size(), QVector<double>(_points.size(), std::numeric_limits<double>::max()));
    _edges = QVector<QVector<bool> >(_points.size(), QVector<bool>(_points.size(), false));

    for(int i = 0; i < _points.size(); ++i)
    {
        for(int j = i + 1; j <_points.size(); ++j)
        {
            if(labels[i] == labels[j])
            {
                continue;
            }
            QLineF l(_points[i], _points[j]);
            if(!_is_blocked(l, obs))
            {
                _edges[i][j] = true;
                _edges[j][i] = true;
                w[i][j] = l.length();
                w[j][i] = l.length();
                _lines.push_back(l);
            }
        }
    }

    for(int i = 0; i < sources.size(); ++i)
    {
        QVector<double> d;
        _dijkstra(_points, w, i, d);
    }
}

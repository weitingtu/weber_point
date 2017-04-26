#include "cdtmanager.h"
#include <QMap>
#include <QSet>

CDTManager::CDTManager(QObject *parent) : QObject(parent),
    _sources(),
    _obstacles(),
    _hexagonals(),
    _points(),
    _source_indices(),
    _obstacle_indices(),
    _hex_indices(),
    _segments(),
    _hole_internal_points(),
    _lines(),
    _triangles(),
    _graph(),
    _source_graph()
{
}

void CDTManager::clear()
{
    _sources.clear();
    _obstacles.clear();
    _hexagonals.clear();
    _points.clear();
    _source_indices.clear();
    _obstacle_indices.clear();
    _hex_indices.clear();
    _segments.clear();
    _hole_internal_points.clear();
    _lines.clear();
    _triangles.clear();
    _graph.clear();
    _source_graph.clear();
}

void CDTManager::cdt()
{
    if(_hexagonals.empty())
    {
        return;
    }

    _data_to_hole_internal_points();
    _data_to_points();
    _points_to_segments();

    struct triangulateio in  = _create_input();
    struct triangulateio mid = _create_mid();

    char options[] = "pczAen";
    triangulate(options, &in, &mid, (struct triangulateio *) NULL );

    _set_lines_by_edges(mid);
    _set_triangles(mid);

    free(in.pointlist);
    free(in.segmentlist);
    free(in.holelist);
    free(mid.pointlist);
    free(mid.pointattributelist);
    free(mid.pointmarkerlist);
    free(mid.trianglelist);
    free(mid.triangleattributelist);
    free(mid.trianglearealist);
    free(mid.neighborlist);
    free(mid.segmentlist);
    free(mid.segmentmarkerlist);
    free(mid.edgelist);
    free(mid.edgemarkerlist);
}

void CDTManager::_data_to_hole_internal_points(const QVector<QPolygonF>& poly)
{
    for(int i = 0; i < poly.size(); ++i)
    {
        if(poly[i].size() < 3)
        {
            continue;
        }
        for(int start_idx = 0; start_idx < poly[i].size() - 3; ++start_idx)
        {
            double x = 0.0;
            double y = 0.0;
            for(int j = start_idx; j < 3; ++j)
            {
                x += poly[i][j].x();
                y += poly[i][j].y();
            }
            x /= 3;
            y /= 3;

            if(poly[i].containsPoint(QPointF(x, y), Qt::OddEvenFill))
            {
                _hole_internal_points.push_back(QPointF(x, y));
                break;;
            }
        }
    }
}

void CDTManager::_data_to_hole_internal_points()
{
    _hole_internal_points.clear();
    _data_to_hole_internal_points(_sources);
    _data_to_hole_internal_points(_obstacles);
}

void CDTManager::_poly_to_points(const QVector<QPolygonF>& poly, QVector<QPointF>& points, QVector<QVector<int> >& indices, int& idx )
{
    indices.clear();
    indices.resize(poly.size());
    for(int i = 0; i < poly.size(); ++i)
    {
        points += poly[i];
        points.pop_back();
        QVector<int> hole_indices;
        for(int j = 0; j < poly[i].size() - 1; ++j, ++idx)
        {
            hole_indices.push_back(idx);
        }
        if(!hole_indices.empty())
        {
            // add start point
            hole_indices.push_back(hole_indices.front());
        }
        indices[i] = hole_indices;
    }
}

void CDTManager::_data_to_points()
{
    _points.clear();
    _hex_indices.clear();
    _hex_indices.resize(_hexagonals.size());
    int idx = 0;
    for(int i = 0; i < _hexagonals.size(); ++i)
    {
        _points += _hexagonals[i];
        for(int j = 0; j < _hexagonals[i].size(); ++j, ++idx)
        {
            _hex_indices[i].push_back(idx);
        }
    }
    _poly_to_points( _sources,   _points, _source_indices,   idx );
    _poly_to_points( _obstacles, _points, _obstacle_indices, idx );
}

void CDTManager::_points_to_segments()
{
    _segments.clear();
    // boundary
    for(int i = 0; i < _hex_indices.front().size() - 1; ++i)
    {
        _segments.push_back(qMakePair(_hex_indices.front()[i], _hex_indices.front()[i + 1]));
    }
    for(int i = 0; i < _hex_indices.back().size() - 1; ++i)
    {
        _segments.push_back(qMakePair(_hex_indices.back()[i], _hex_indices.back()[i + 1]));
    }
    for(int i = 0; i < _hex_indices.size() - 1; i += 2)
    {
        _segments.push_back(qMakePair(_hex_indices[i].front(), _hex_indices[i + 2].front()));
        _segments.push_back(qMakePair(_hex_indices[i].back(),  _hex_indices[i + 2].back()));
    }

    for(int i = 0; i < _source_indices.size(); ++i)
    {
        for(int j = 0; j <_source_indices[i].size() - 1; ++j)
        {
            _segments.push_back(qMakePair(_source_indices[i][j], _source_indices[i][j + 1]));
        }
    }

    for(int i = 0; i < _obstacle_indices.size(); ++i)
    {
        for(int j = 0; j <_obstacle_indices[i].size() - 1; ++j)
        {
            _segments.push_back(qMakePair(_obstacle_indices[i][j], _obstacle_indices[i][j + 1]));
        }
    }
}

struct triangulateio CDTManager::_create_input() const
{
    struct triangulateio in;
    in.numberofpoints = _points.size();
    in.numberofpointattributes = 0;
    in.pointlist = (REAL*) malloc(in.numberofpoints * 2 * sizeof(REAL));
    for(int i = 0; i < _points.size(); ++i)
    {
        in.pointlist[i * 2]     = _points[i].x();
        in.pointlist[i * 2 + 1] = _points[i].y();
    }
    in.numberofsegments = _segments.size();
    in.segmentlist = (int*) malloc((in.numberofsegments * 2 * sizeof(int)));
    for(int i = 0; i < _segments.size(); ++i)
    {
        in.segmentlist[i * 2]     = _segments[i].first;
        in.segmentlist[i * 2 + 1] = _segments[i].second;
    }
    in.numberofholes = _hole_internal_points.size();
    in.holelist = (REAL*) malloc(in.numberofholes * 2 * sizeof(REAL));
    for(int i = 0; i < _hole_internal_points.size(); ++i)
    {
        in.holelist[i * 2]     = _hole_internal_points[i].x();
        in.holelist[i * 2 + 1] = _hole_internal_points[i].y();
    }
    in.numberofregions = 0;

    return in;
}

struct triangulateio CDTManager::_create_mid() const
{
    struct triangulateio mid;
    mid.pointlist = (REAL *) NULL;             /* Not needed if -N switch used. */
    mid.pointattributelist = (REAL *) NULL;    /* Not needed if -N switch used or number of point attributes is zero: */
    mid.pointmarkerlist = (int *) NULL;        /* Not needed if -N or -B switch used. */
    mid.trianglelist = (int *) NULL;           /* Not needed if -E switch used. */
    mid.triangleattributelist = (REAL *) NULL; /* Not needed if -E switch used or number of triangle attributes is zero: */
    mid.neighborlist = (int *) NULL;           /* Needed only if -n switch used. */
    mid.segmentlist = (int *) NULL;            /* Needed only if segments are output (-p or -c) and -P not used: */
    mid.segmentmarkerlist = (int *) NULL;      /* Needed only if segments are output (-p or -c) and -P and -B not used: */
    mid.edgelist = (int *) NULL;               /* Needed only if -e switch used. */
    mid.edgemarkerlist = (int *) NULL;         /* Needed if -e used and -B not used. */

    return mid;
}

void CDTManager::_set_lines_by_triangles(const triangulateio& io)
{
    _lines.clear();
    for (int i = 0; i < io.numberoftriangles; ++i) {
        int idx1 = io.trianglelist[i * io.numberofcorners] * 2;
        int idx2 = io.trianglelist[i * io.numberofcorners + 1] * 2;
        int idx3 = io.trianglelist[i * io.numberofcorners + 2] * 2;
        _lines.push_back(QLineF(QPointF(io.pointlist[idx1], io.pointlist[idx1 +1]), QPointF(io.pointlist[idx2], io.pointlist[idx2 + 1])));
        _lines.push_back(QLineF(QPointF(io.pointlist[idx2], io.pointlist[idx2 +1]), QPointF(io.pointlist[idx3], io.pointlist[idx3 + 1])));
        _lines.push_back(QLineF(QPointF(io.pointlist[idx3], io.pointlist[idx3 +1]), QPointF(io.pointlist[idx1], io.pointlist[idx1 + 1])));
    }
}

void CDTManager::_set_lines_by_edges(const triangulateio& io)
{
    _lines.clear();
    for (int i = 0; i < io.numberofedges; i++) {
        int idx1 = io.edgelist[i * 2] * 2;
        int idx2 = io.edgelist[i * 2 + 1] * 2;
        _lines.push_back(QLineF(QPointF(io.pointlist[idx1], io.pointlist[idx1 +1]), QPointF(io.pointlist[idx2], io.pointlist[idx2 + 1])));
    }
}

void CDTManager::_set_triangles(const triangulateio& io)
{
    _triangles.clear();
    for (int i = 0; i < io.numberoftriangles; i++)
    {
        Triangle t;
        for (int j = 0; j < io.numberofcorners; j++)
        {
            int idx = io.trianglelist[i * io.numberofcorners + j];
//            t.points[j] = QPointF(io.pointlist[idx * 2], io.pointlist[idx * 2 + 1 ]);
            t.points[j] = _points[idx];
            t.indices[j]  = idx;
        }

        for (int j = 0; j < 3; j++)
        {
            t.neighbors[j] = io.neighborlist[i * 3 + j];
        }
        _triangles.push_back(t);
    }
}

QMap<int, QVector<int> > CDTManager::_build_triangle_point_map(const QVector<Triangle>& triangles) const
{
    QMap<int, QVector<int> > map;
    for(int i = 0;i < triangles.size(); ++i)
    {
        const Triangle& t = triangles[i];
        for(int j = 0; j < 3; ++j)
        {
            if(t.indices[j] >= 0)
            {
                map[t.indices[j]].push_back(i);
            }
        }
    }

    return map;
}

QVector<QVector<int>> CDTManager::_build_poly_neighbor_triangles( const QMap<int, QVector<int> >& map ,const QVector<QVector<int> >& source_indices ) const
{
    QVector<QVector<int>> source_neighbors;
    source_neighbors.resize(source_indices.size());

    for(int i = 0; i < source_indices.size(); ++i)
    {
        QMap<int, int> count;
        for(int j = 0; j < source_indices[i].size() - 1; ++j)
        {
            int idx = source_indices[i][j];
            if(!map.contains(idx))
            {
                continue;
            }
            const QVector<int>& v = map[idx];
            for(int k = 0; k < v.size(); ++k)
            {
                count[v[k]]++;
            }
        }

        QMap<int, int>::const_iterator ite     = count.constBegin();
        QMap<int, int>::const_iterator ite_end = count.constEnd();
        for(; ite != ite_end; ++ite)
        {
            if(ite.value() != 2)
            {
                continue;
            }
            source_neighbors[i].push_back(ite.key());
        }
    }
    return source_neighbors;
}

void CDTManager::fermat_point()
{
    QMap<int, QVector<int> > triangle_point_map = _build_triangle_point_map(_triangles);

    _graph.clear();
    _graph.resize(_triangles.size());

    int idx = 0;
    for( int i = 0; i < _triangles.size(); ++i, ++idx )
    {
        const Triangle&t = _triangles[i];
        for(int j = 0; j < 3; ++j)
        {
            _graph[idx].points.push_back(t.points[j]);
            _graph[idx].indices.push_back(t.indices[j]);
            if(t.neighbors[j] >= 0)
            {
                _graph[idx].neighbors.push_back(t.neighbors[j]);
            }
        }
//        QPointF fermat_point = FermatPoint::CalcFermatPoint(triangles[i]);
        _graph[idx].center = QPointF((t.points[0].x() + t.points[1].x() + t.points[2].x()) / 3, (t.points[0].y() + t.points[1].y() + t.points[2].y()) / 3);
    }

    QVector<QVector<int>> source_neighbor_triangles = _build_poly_neighbor_triangles(triangle_point_map, _source_indices);
    _source_graph.clear();
    _source_graph.resize(_source_indices.size());

    for(int i = 0; i < _source_indices.size(); ++i, ++idx)
    {
        double x = 0.0;
        double y = 0.0;
        for(int j = 0; j < _source_indices[i].size() - 1; ++j)
        {
            int index = _source_indices[i][j];
            _source_graph[i].indices.push_back(index);
            _source_graph[i].points.push_back(_points[index]);
            x += _points[index].x();
            y += _points[index].y();
        }
        x /= ( _source_indices[i].size() - 1 );
        y /= ( _source_indices[i].size() - 1 );
        _source_graph[i].center = QPointF(x, y);

        for(int j = 0; j < source_neighbor_triangles[i].size(); ++j)
        {
           _source_graph[i] .neighbors.push_back(source_neighbor_triangles[i][j]);
        }
    }
}


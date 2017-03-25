#include "cdtmanager.h"

CDTManager::CDTManager(QObject *parent) : QObject(parent),
    _holes(),
    _hexagonals(),
    _points(),
    _hole_indices(),
    _hex_indices(),
    _segments(),
    _hole_points(),
    _lines()
{
}

void CDTManager::clear()
{
    _holes.clear();
    _hexagonals.clear();
    _points.clear();
    _hole_indices.clear();
    _hex_indices.clear();
    _segments.clear();
    _hole_points.clear();
    _lines.clear();
}

void CDTManager::cdt()
{
    if(_hexagonals.empty())
    {
        return;
    }

    _data_to_hole_points();
    _data_to_points();
    _points_to_segments();

    struct triangulateio in = _create_input();
    struct triangulateio mid = _create_mid();

    char options[] = "pczAen";
    triangulate(options, &in, &mid, (struct triangulateio *) NULL );

    _set_lines_by_edges(mid);

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

void CDTManager::_data_to_hole_points()
{
    _hole_points.clear();
    for(int i = 0; i < _holes.size(); ++i)
    {
        double x = _holes[i][0].x();
        double y = _holes[i][0].y();
        for(int j = 1; j < _holes[i].size() - 1; ++j)
        {
            x += _holes[i][j].x();
            y += _holes[i][j].y();
        }
        x /= (_holes[i].size() - 1);
        y /= (_holes[i].size() - 1);
        _hole_points.push_back(QPointF(x, y));
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
    _hole_indices.clear();
    _hole_indices.resize(_holes.size());
    for(int i = 0; i < _holes.size(); ++i)
    {
        _points += _holes[i];
        _points.pop_back();
        QVector<int> hole_indices;
        for(int j = 0; j < _holes[i].size() - 1; ++j, ++idx)
        {
            hole_indices.push_back(idx);
        }
        if(!hole_indices.empty())
        {
            // add start point
            hole_indices.push_back(hole_indices.front());
        }
        _hole_indices[i] = hole_indices;
    }
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

    for(int i = 0; i < _hole_indices.size(); ++i)
    {
        for(int j = 0; j <_hole_indices[i].size() - 1; ++j)
        {
            _segments.push_back(qMakePair(_hole_indices[i][j], _hole_indices[i][j + 1]));
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
    in.numberofholes = _hole_points.size();
    in.holelist = (REAL*) malloc(in.numberofholes * 2 * sizeof(REAL));
    for(int i = 0; i < _hole_points.size(); ++i)
    {
        in.holelist[i * 2]     = _hole_points[i].x();
        in.holelist[i * 2 + 1] = _hole_points[i].y();
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

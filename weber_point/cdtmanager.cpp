#include "cdtmanager.h"

CDTManager::CDTManager(QObject *parent) : QObject(parent),
    _holes(),
    _hexagonals(),
    _points(),
    _hole_points(),
    _hex_points(),
    _lines()
{
}

void CDTManager::cdt()
{
    if(_hexagonals.empty())
    {
        return;
    }

    _data_to_points();

    struct triangulateio in = _create_input();
    struct triangulateio mid = _create_mid();

    char options[] = "pczAen";
    triangulate(options, &in, &mid, (struct triangulateio *) NULL );

    _set_lines_by_edges(mid);

    free(in.pointlist);
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

void CDTManager::_data_to_points()
{
    _points.clear();
    _hex_points.clear();
    _hex_points.resize(_hexagonals.size());
    int idx = 0;
    for(int i = 0; i < _hexagonals.size(); ++i)
    {
        _points += _hexagonals[i];
        for(int j = 0; j < _hexagonals[i].size(); ++j, ++idx)
        {
            _hex_points[i].push_back(idx);
        }
    }
    _hole_points.clear();
    _hole_points.resize(_holes.size());
    for(int i = 0; i < _holes.size(); ++i)
    {
        _points += _holes[i];
        _points.pop_back();
        for(int j = 0; j < _holes[i].size() - 1; ++j, ++idx)
        {
            _hole_points[i].push_back(idx);
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
    in.numberofsegments = 0;
    in.numberofholes = 0;
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

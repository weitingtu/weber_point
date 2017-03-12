#include "cdtmanager.h"

CDTManager::CDTManager(QObject *parent) : QObject(parent),
    _holes(),
    _hex_points()
{
}

void CDTManager::cdt()
{
    if(_hex_points.empty())
    {
        return;
    }

    struct triangulateio in = _create_input();
    struct triangulateio mid = _create_mid();

    char options[] = "pczAen";
    triangulate(options, &in, &mid, (struct triangulateio *) NULL );

    _set_segments(mid);

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

int CDTManager::_get_num_of_points() const
{
    int num_of_points = 0;
    for(int i = 0; i < _hex_points.size(); ++i)
    {
        num_of_points += _hex_points[i].size();
    }

    for(int i = 0; i < _holes.size(); ++i)
    {
        // start point and end point are the same
        num_of_points += (_holes[i].size() - 1);
    }

    return num_of_points;
}

void CDTManager::_set_points(REAL* pointlist) const
{
    QVector<QPointF> points;
    for(int i = 0; i < _hex_points.size(); ++i)
    {
        points += _hex_points[i];
    }
    for(int i = 0; i < _holes.size(); ++i)
    {
        points += _holes[i];
        points.pop_back();
    }

    for(int i = 0; i < points.size(); ++i)
    {
        pointlist[i * 2]     = points[i].x();
        pointlist[i * 2 + 1] = points[i].y();
    }
}

struct triangulateio CDTManager::_create_input() const
{
    struct triangulateio in;
    in.numberofpoints = _get_num_of_points();
    in.numberofpointattributes = 0;
    in.pointlist = (REAL*) malloc(in.numberofpoints * 2 * sizeof(REAL));
    _set_points(in.pointlist);
    in.numberofsegments = 0;
    in.numberofholes = 0;
    in.numberofregions = 0;

    return in;
}

struct triangulateio CDTManager::_create_mid() const
{
    struct triangulateio mid;
    mid.pointlist = (REAL *) NULL;            /* Not needed if -N switch used. */
    /* Not needed if -N switch used or number of point attributes is zero: */
    mid.pointattributelist = (REAL *) NULL;
    mid.pointmarkerlist = (int *) NULL; /* Not needed if -N or -B switch used. */
    mid.trianglelist = (int *) NULL;          /* Not needed if -E switch used. */
    /* Not needed if -E switch used or number of triangle attributes is zero: */
    mid.triangleattributelist = (REAL *) NULL;
    mid.neighborlist = (int *) NULL;         /* Needed only if -n switch used. */
    /* Needed only if segments are output (-p or -c) and -P not used: */
    mid.segmentlist = (int *) NULL;
    /* Needed only if segments are output (-p or -c) and -P and -B not used: */
    mid.segmentmarkerlist = (int *) NULL;
    mid.edgelist = (int *) NULL;             /* Needed only if -e switch used. */
    mid.edgemarkerlist = (int *) NULL;   /* Needed if -e used and -B not used. */

    return mid;
}

void CDTManager::_set_segments(const triangulateio& io)
{
    _lines.clear();
    for (int i = 0; i < io.numberoftriangles; ++i) {
        for (int j = 0; j < io.numberofcorners - 1; ++j) {
            int idx1 = io.trianglelist[i * io.numberofcorners + j] * 2;
            int idx2 = io.trianglelist[i * io.numberofcorners + j + 1] * 2;
            _lines.push_back(QLineF(QPointF(io.pointlist[idx1], io.pointlist[idx1 +1]), QPointF(io.pointlist[idx2], io.pointlist[idx2 + 1])));
        }
    }
}

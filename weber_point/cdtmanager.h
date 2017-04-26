#ifndef CDTMANAGER_H
#define CDTMANAGER_H

#ifdef SINGLE
#define REAL float
#else /* not SINGLE */
#define REAL double
#endif /* not SINGLE */
#define VOID int
#include "triangulation.h"

#include "triangle.h"
#include "poly.h"
#include <QObject>
#include <QPolygonF>
#include <QPointF>
#include <QLineF>
#include <QVector>

class CDTManager : public QObject
{
    Q_OBJECT
public:
    static CDTManager& get_inst()
    {
        static CDTManager inst;
        return inst;
    }

    void add_sources(const QVector<QPolygonF>& sources) { _sources += sources; }
    void add_obstacles(const QVector<QPolygonF>& obstacles) { _obstacles += obstacles; }
    void add_hexagonals(const QVector<QPointF>& points) { _hexagonals.push_back(points); }
    void clear();
    void cdt();
    void fermat_point();

    bool is_hexagonals_empty() const { return _hexagonals.empty(); }

    const QVector<QLineF>&   get_lines() const { return _lines; }
    const QVector<Triangle>& get_triangles() const { return _triangles; }

    const QVector<Poly>&     get_graph() const { return _graph; }
    const QVector<Poly>&     get_source_graph() const { return _source_graph; }
signals:

public slots:
private:
    explicit CDTManager(QObject *parent = 0);

    void _data_to_hole_internal_points(const QVector<QPolygonF>& poly);
    void _data_to_hole_internal_points();
    void _poly_to_points(const QVector<QPolygonF>& poly, QVector<QPointF> &points, QVector<QVector<int> >& indices , int &idx);
    void _data_to_points();
    void _points_to_segments();
    struct triangulateio _create_input() const;
    struct triangulateio _create_mid() const;
    void _set_lines_by_triangles(const triangulateio& io);
    void _set_lines_by_edges(const triangulateio& io);
    void _set_triangles(const triangulateio& io);

    QMap<int, QVector<int> > _build_triangle_point_map(const QVector<Triangle> &triangles) const;
    QVector<QVector<int>> _build_poly_neighbor_triangles( const QMap<int, QVector<int> >& map ,const QVector<QVector<int> >& source_indices ) const;

    // raw data
    QVector<QPolygonF>         _sources;
    QVector<QPolygonF>         _obstacles;
    QVector<QVector<QPointF> > _hexagonals;

    QVector<QPointF>           _points;
    QVector<QVector<int> >     _source_indices;
    QVector<QVector<int> >     _obstacle_indices;
    QVector<QVector<int> >     _hex_indices;
    QVector<QPair<int, int> >  _segments;
    QVector<QPointF>           _hole_internal_points;

    // output
    QVector<QLineF>            _lines;
    QVector<Triangle>          _triangles;

    QVector<Poly>              _graph;
    QVector<Poly>              _source_graph;
};

inline CDTManager& get_cdt_manager()
{
    return CDTManager::get_inst();
}

#endif // CDTMANAGER_H

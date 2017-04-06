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
    explicit CDTManager(QObject *parent = 0);

    void add_holes(const QVector<QPolygonF>& holes) { _holes += holes; }
    void add_hexagonals(const QVector<QPointF>& points) { _hexagonals.push_back(points); }
    void clear();
    void cdt();
    void fermat_point();

    const QVector<QLineF>&   get_lines() const { return _lines; }
    const QVector<Triangle>& get_triangles() const { return _triangles; }
signals:

public slots:
private:
    void _data_to_hole_center_points();
    void _data_to_points();
    void _points_to_segments();
    struct triangulateio _create_input() const;
    struct triangulateio _create_mid() const;
    void _set_lines_by_triangles(const triangulateio& io);
    void _set_lines_by_edges(const triangulateio& io);
    void _set_triangles(const triangulateio& io);

    // raw data
    QVector<QPolygonF>         _holes;
    QVector<QVector<QPointF> > _hexagonals;

    QVector<QPointF>           _points;
    QVector<QVector<int> >     _hole_indices;
    QVector<QVector<int> >     _hex_indices;
    QVector<QPair<int, int> >  _segments;
    QVector<QPointF>           _hole_center_points;

    // output
    QVector<QLineF>            _lines;
    QVector<Triangle>          _triangles;
};

inline CDTManager& get_cdt_manager()
{
    return CDTManager::get_inst();
}

#endif // CDTMANAGER_H

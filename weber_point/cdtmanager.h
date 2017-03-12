#ifndef CDTMANAGER_H
#define CDTMANAGER_H

#ifdef SINGLE
#define REAL float
#else /* not SINGLE */
#define REAL double
#endif /* not SINGLE */
#define VOID int
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
    void add_hex_points(const QVector<QPointF>& points) { _hex_points.push_back(points); }
    void clear() { _holes.clear(); _hex_points.clear(); _lines.clear(); }
    void cdt();

    const QVector<QLineF>& get_lines() const { return _lines; }
signals:

public slots:
private:
    int _get_num_of_points() const;
    void _set_points(REAL* pointlist) const;
    struct triangulateio _create_input() const;
    struct triangulateio _create_mid() const;
    void _set_lines(const triangulateio& io);

    QVector<QPolygonF>         _holes;
    QVector<QVector<QPointF> > _hex_points;
    QVector<QLineF>            _lines;
};

inline CDTManager& get_cdt_manager()
{
    return CDTManager::get_inst();
}

#endif // CDTMANAGER_H

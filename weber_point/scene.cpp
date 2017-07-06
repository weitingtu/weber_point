#include "scene.h"
#include "inputmanager.h"
#include "poly.h"
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsTextItem>

Scene::Scene(QObject *parent ):
    QGraphicsScene(parent),
    _mode(MODE::CREATE_SOURCE_RECT),
    _accept_input(true),
    _points(),
    _texts(),
    _vg_lines(),
    _hex_points(),
    _cdt_lines(),
    _fermat_points(),
    _fermat_lines()
{
    initialize();
}

void Scene::clear_all()
{
    clear();
    _accept_input = true;
    _points.clear();
    _texts.clear();
    _vg_lines.clear();
    _hex_points.clear();
    _cdt_lines.clear();
    _fermat_points.clear();
    _fermat_lines.clear();
}

void Scene::add_text(const QPointF& p, const QString& text)
{
    QGraphicsTextItem* text_item = addText(text);
    text_item->setX(p.x());
    text_item->setY(p.y());
    _texts.push_back(text_item);
}

void Scene::clear_texts()
{
    for(int i = 0; i < _texts.size(); ++i)
    {
        removeItem(_texts[i]);
        delete(_texts[i]);
    }
    _texts.clear();
}

void Scene::adjust_texts(int size)
{
    for(int i = 0; i < _texts.size(); ++i)
    {
        QFont f = _texts[i]->font();
        f.setPointSize(size);
        _texts[i]->setFont(f);
    }
}

void Scene::add_vg_lines(const QVector<QLineF>& lines)
{
    for(const QLineF& l : lines)
    {
        _vg_lines.push_back(addLine(l, QPen(QColor(Qt::blue))));
    }
}

void Scene::add_vg_pathes(const QVector<QLineF>& lines)
{
    for(const QLineF& l : lines)
    {
        _vg_lines.push_back(addLine(l, QPen(QColor(Qt::magenta))));
    }
}

void Scene::clear_vg_lines()
{
    for(int i = 0; i < _vg_lines.size(); ++i)
    {
        removeItem(_vg_lines[i]);
        delete(_vg_lines[i]);
    }
    _vg_lines.clear();
}

void Scene::add_hex_point(double x, double y, double rad)
{
    _hex_points.push_back(addEllipse(x - rad, y - rad, rad * 2, rad * 2));
}

void Scene::clear_hex_points()
{
    for(int i = 0; i < _hex_points.size(); ++i)
    {
        removeItem(_hex_points[i]);
        delete(_hex_points[i]);
    }
    _hex_points.clear();
}

void Scene::add_cdt_lines(const QVector<QLineF>& lines)
{
    for(const QLineF& l : lines)
    {
        _cdt_lines.push_back(addLine(l));
    }
}

void Scene::clear_cdt_lines()
{
    for(int i = 0; i < _cdt_lines.size(); ++i)
    {
        removeItem(_cdt_lines[i]);
        delete(_cdt_lines[i]);
    }
    _cdt_lines.clear();
}

void Scene::add_fermat_point(double x, double y, double rad)
{
    _fermat_points.push_back(addEllipse(x - rad, y - rad, rad * 2, rad * 2));
}

void Scene::add_fermat_line(const QLineF &line)
{
    _fermat_lines.push_back(addLine(line, QPen(QColor(Qt::green))));
}

void Scene::clear_fermat()
{
    for(int i = 0; i < _fermat_points.size(); ++i)
    {
        removeItem(_fermat_points[i]);
        delete(_fermat_points[i]);
    }
    _fermat_points.clear();

    for(int i = 0; i < _fermat_lines.size(); ++i)
    {
        removeItem(_fermat_lines[i]);
        delete(_fermat_lines[i]);
    }
    _fermat_lines.clear();
}

void Scene::initialize()
{
    double width  = get_input_manager().get_width();
    double height = get_input_manager().get_height();
    double w = width * 0.1;
    double h = height * 0.1;
    setSceneRect(0 - w, 0 - h, width + 2 * w, height + 2 * h);
    addRect(0, 0, width, height);

    const QVector<QPolygonF>& sources = get_input_manager().get_sources();
    for(int i = 0; i < sources.size(); ++i)
    {
        addPolygon(sources[i], QPen(), QBrush(Qt::gray));
    }
    const QVector<QPolygonF>& obstacles = get_input_manager().get_obstacles();
    for(int i = 0; i < obstacles.size(); ++i)
    {
        addPolygon(obstacles[i], QPen(), QBrush(Qt::black));
    }
}

void Scene::set_mode( MODE m)
{
    _mode = m;
    _points.clear();
}

namespace  {

QRectF _create_rectf(const QPointF& p1, const QPointF& p2)
{
     qreal x = qMin(p1.x(), p2.x());
     qreal y = qMin(p1.y(), p2.y());
     qreal w = qAbs(p1.x() - p2.x());
     qreal h = qAbs(p1.y() - p2.y());

     return QRectF(x, y, w, h);
}

bool _is_valid(const QPolygonF& p)
{
    for(int i = 0; i < p.size() - 1; ++i)
    {
        int p1 = i;
        int p2 = i + 1;
        if( p2 == p.size() - 1)
        {
            p2 = 0;
        }

        QLineF l1(p[p1], p[p2]);
        for(int j = 0; j < p.size() - 1; ++j)
        {
            int p3 = j;
            int p4 = j + 1;
            if( p4 == p.size() - 1)
            {
                p4 = 0;
            }

            if(( p1 == p3 ) || ( p1 == p4 ) || ( p2 == p3 ) || ( p2 == p4 ))
            {
                continue;
            }

            QLineF l2(p[p3], p[p4]);
            QPointF p;
            if(QLineF::BoundedIntersection == l1.intersect(l2, &p))
            {
                return false;
            }
        }
    }
    return true;
}

}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if( _accept_input && (( MODE::CREATE_SOURCE_RECT == _mode ) || ( MODE::CREATE_OBS_RECT == _mode ))
            && ( mouseEvent->button()==Qt::LeftButton )
            && (sceneRect().contains(mouseEvent->scenePos()))
            )
    {
        if(get_input_manager().is_blocked(mouseEvent->scenePos()))
        {
            _points.clear();
            QGraphicsScene::mousePressEvent(mouseEvent);
            return;
        }
        _points.push_back(mouseEvent->scenePos());
        if( 2 == _points.size() )
        {
            QRectF rect = _create_rectf(_points.front(), _points.back());
            if(!get_input_manager().is_blocked(QPolygonF(rect)))
            {
                if( MODE::CREATE_SOURCE_RECT == _mode )
                {
                    addRect(rect, QPen(), QBrush(Qt::gray));
                    get_input_manager().add_source(QPolygonF(rect));
                }
                else if( MODE::CREATE_OBS_RECT == _mode )
                {
                    addRect(rect, QPen(), QBrush(Qt::black));
                    get_input_manager().add_obstacle(QPolygonF(rect));
                }
            }
            _points.clear();
        }
    }
    else if( _accept_input
             && (( MODE::CREATE_SOURCE_POLY == _mode ) || ( MODE::CREATE_OBS_POLY == _mode ))
             && !get_input_manager().is_blocked(mouseEvent->scenePos())
             )
    {
        if(( mouseEvent->button()==Qt::LeftButton ) && (sceneRect().contains(mouseEvent->scenePos())))
        {
            _points.push_back(mouseEvent->scenePos());
        }
        else if(( mouseEvent->button()==Qt::RightButton ) && (sceneRect().contains(mouseEvent->scenePos())))
        {
            if( 2 < _points.size() )
            {
                _points.push_back(_points.front());
                QPolygonF poly(_points);
                if( _is_valid(poly) && !get_input_manager().is_blocked(poly) )
                {
                    if( MODE::CREATE_SOURCE_POLY == _mode )
                    {
                        addPolygon( poly, QPen(), QBrush(Qt::gray));
                        get_input_manager().add_source( poly );
                    }
                    else if( MODE::CREATE_OBS_POLY == _mode )
                    {
                        addPolygon( poly, QPen(), QBrush(Qt::black));
                        get_input_manager().add_obstacle( poly );
                    }
                }
            }
            _points.clear();
        }
    }
    QGraphicsScene::mousePressEvent(mouseEvent);
}

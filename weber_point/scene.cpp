#include "scene.h"
#include "inputmanager.h"
#include "poly.h"
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsTextItem>

Scene::Scene(QObject *parent ):
    QGraphicsScene(parent),
    _mode(MODE::CREATE_SOURCE_RECT),
    _points(),
    _texts()
{
    initialize();
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
    }
    _texts.clear();
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
    if( (( MODE::CREATE_SOURCE_RECT == _mode ) || ( MODE::CREATE_OBS_RECT == _mode ))
            && ( mouseEvent->button()==Qt::LeftButton ) && (sceneRect().contains(mouseEvent->scenePos())) )
    {
        _points.push_back(mouseEvent->scenePos());
        if( 2 == _points.size() )
        {
            QRectF rect = _create_rectf(_points.front(), _points.back());
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
            _points.clear();
        }
    }
    else if( ( MODE::CREATE_SOURCE_POLY == _mode ) || ( MODE::CREATE_OBS_POLY == _mode ))
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
                if( _is_valid(poly) )
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

#include "scene.h"
#include "inputmanager.h"
#include <QGraphicsSceneMouseEvent>

Scene::Scene(QObject *parent ):
    QGraphicsScene(parent),
    _mode(MODE::CREATE_SOURCE_RECT),
    _points()
{
    initialize();
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
            _points.clear();
        }
    }
    QGraphicsScene::mousePressEvent(mouseEvent);
}

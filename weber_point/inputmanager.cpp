#include "inputmanager.h"

InputManager::InputManager(QObject *parent) : QObject(parent),
    _width(600),
    _height(300),
    _sources(),
    _obstacles()
{
    {
        QPolygonF poly(QVector<QPointF>{QPointF(10, 10), QPoint(20, 10), QPoint(20, 20), QPoint(10, 20), QPoint(10, 10)});
        _sources << poly;
    }
    {
        QPolygonF poly(QVector<QPointF>{QPointF(100, 100), QPoint(100, 150), QPoint(150, 150), QPoint(150, 100), QPoint(100, 100)});
        _obstacles << poly;
    }
}

bool InputManager::is_blocked(const QPointF& point) const
{
    for(int i = 0; i < _sources.size(); ++i)
    {
        if(_sources[i].containsPoint(point, Qt::OddEvenFill))
        {
            return true;
        }
    }
    for(int i = 0; i < _obstacles.size(); ++i)
    {
        if(_obstacles[i].containsPoint(point, Qt::OddEvenFill))
        {
            return true;
        }
    }
    return false;
}

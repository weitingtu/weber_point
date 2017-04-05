#include "inputmanager.h"

InputManager::InputManager(QObject *parent) : QObject(parent),
    _width(600),
    _height(300),
    _sources(),
    _obstacles()
{
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

double InputManager::get_source_area() const
{
    double area = 0;
    for(int i = 0; i < _sources.size(); ++i)
    {
        QRectF r = _sources[i].boundingRect();
        area += r.width() * r.height();
    }
    return area;
}

double InputManager::get_total_area() const
{
    return _width * _height;
}

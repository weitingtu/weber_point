#include "inputmanager.h"

static const int _default_width(600);
static const int _default_height(300);

InputManager::InputManager(QObject *parent) : QObject(parent),
    _width(_default_width),
    _height(_default_height),
    _sources(),
    _obstacles()
{
}

void InputManager::clear()
{
    _width  = _default_width;
    _height = _default_height;
    _sources.clear();
    _obstacles.clear();
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

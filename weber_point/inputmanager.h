#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <QObject>
#include <QVector>
#include <QPolygonF>

class InputManager : public QObject
{
    Q_OBJECT
public:
    static InputManager& get_inst()
    {
        static InputManager inst;
        return inst;
    }
    explicit InputManager(QObject *parent = 0);

    void set_width(double width) { _width = width; }
    void set_height(double height) { _height = height; }
    double get_width() const { return _width; }
    double get_height() const { return _height; }
    const QVector<QPolygonF>& get_sources() const { return _sources; }
    const QVector<QPolygonF>& get_obstacles() const { return _obstacles; }
    bool is_blocked(const QPointF& point) const;
signals:

public slots:
private:
    double _width;
    double _height;
    QVector<QPolygonF> _sources;
    QVector<QPolygonF> _obstacles;
};

inline InputManager& get_input_manager()
{
    return InputManager::get_inst();
}

#endif // INPUTMANAGER_H

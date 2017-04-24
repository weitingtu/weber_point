#ifndef SCENE_H
#define SCENE_H

#include "enum.h"
#include <QGraphicsScene>
#include <QPointF>
#include <QVector>

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Scene(QObject*parent = 0);

    void add_text(const QPointF& p, const QString& text);
    void clear_texts();
    void adjust_texts();

signals:

public slots:
    void set_mode(MODE m);
    void initialize();
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;

private:
    MODE _mode;
    QVector<QPointF>           _points;
    QVector<QGraphicsTextItem*> _texts;
};

#endif // SCENE_H

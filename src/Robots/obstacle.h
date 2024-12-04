/**
* @file obstacle.h
* @author Ondrej Janecka
* @author Rostyslav Kachan
*/

#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "qgraphicsitem.h"

class Obstacle : public QGraphicsItem
{
private:
    QPointF position;

public:
    Obstacle(QPointF pos);
    QPointF getPosition();
    static Obstacle* create(QPointF);

    // Metody pro QGraphicsItem
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
};

#endif // OBSTACLE_H

/**
* @file robot.h
* @author Ondrej Janecka
* @author Rostyslav Kachan
*/

#ifndef ROBOT_H
#define ROBOT_H

#include "obstacle.h"
#include <QGraphicsItem>
#include <vector>

class Robot : public QGraphicsItem
{
private:
    int direction = 0;
    QPointF position;
    bool enabled = true;
    int moveDistance = 3;
    int triangleBase = 40;

public:
    Robot(QPointF pos);
    static Robot* create(QPointF);
    QPointF getPosition();
    bool canMove(std::vector<Robot*> &robots, std::vector<Obstacle*> &obstacles, QPointF size);
    bool move();
    void turn(int times);
    int angle();
    int getBase();
    void setBase(double size);
    bool isEnabled();
    void switchEnabled();
    // Metody pro QGraphicsItem
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
};

#endif // ROBOT_H

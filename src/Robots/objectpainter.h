/**
* @file objectpainter.h
* @author Ondrej Janecka
* @author Rostyslav Kachan
*/

#ifndef OBJECTPAINTER_H
#define OBJECTPAINTER_H

#include "customgraphicsscene.h"
#include "mappainter.h"

class ObjectPainter
{
public:
    static void PaintRobot(CustomGraphicsScene *scene, Robot *robot);
    static void PaintRobot(MapPainter *scene, Robot *robot, int num);
    static void PaintObstacle(CustomGraphicsScene *scene, Obstacle *obstacle);
    static void PaintObstacle(MapPainter *scene, Obstacle *obstacle);
    static void RemoveObject(CustomGraphicsScene *scene, QPointF scenePos);
};

#endif // OBJECTPAINTER_H

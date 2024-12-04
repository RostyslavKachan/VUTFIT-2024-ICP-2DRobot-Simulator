/**
* @file mappainter.h
* @author Ondrej Janecka
* @author Rostyslav Kachan
*/

#ifndef MAPPAINTER_H
#define MAPPAINTER_H

#include "environment.h"
#include <QGraphicsScene>

class MapPainter : public QGraphicsScene
{
public:
    int width;
    int height;
    explicit MapPainter(QObject *parent = nullptr);
    void PaintMap(Environment &environment);

private:
    void paintObstacles(Environment &environment);
    void paintRobots(Environment &environment);
};

#endif // MAPPAINTER_H

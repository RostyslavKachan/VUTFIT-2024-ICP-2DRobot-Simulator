/**
* @file customgraphicsscene.h
* @author Ondrej Janecka
* @author Rostyslav Kachan
*/

#ifndef CUSTOMGRAPHICSSCENE_H
#define CUSTOMGRAPHICSSCENE_H

#include "obstacle.h"
#include "robot.h"
#include <QGraphicsScene>

class CustomGraphicsScene : public QGraphicsScene
{
    Q_OBJECT

public:
    int width;
    int height;
    explicit CustomGraphicsScene(QObject *parent = nullptr); // Konstruktor
    void CreateRoom(int width, int height);
    static void SetActive(int active);
    void SaveScene(std::string mapName);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
private:
    static int activeRadio;
    std::vector<Obstacle*> obstacles;
    std::vector<Robot*> robots;
    int checkPosition(QPointF scenePos);
    void AddObstacle(QPointF);
    void AddControlledRobot(QPointF scenePos);
    void DeleteObject(QPointF);
};

#endif // CUSTOMGRAPHICSSCENE_H

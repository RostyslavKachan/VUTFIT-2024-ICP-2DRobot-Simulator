/**
* @file customgraphicsscene.cpp
* @brief Implementation of the CustomGraphicsScene class, that handles the scene for creating a new room and saving it to a file.
* @author Ondrej Janecka
* @author Rostyslav Kachan
*/

#include "customgraphicsscene.h"
#include "QGraphicsSceneMouseEvent"
#include "objectpainter.h"
#include "qgraphicsitem.h"
#include <fstream>

int CustomGraphicsScene::activeRadio = 0;

/**
 * @brief Constructor for the CustomGraphicsScene class.
 * @param parent The parent QObject.
 */
CustomGraphicsScene::CustomGraphicsScene(QObject *parent) :
    QGraphicsScene(parent)
{}

/**
 * @brief Handles mouse press events within the scene, adding or deleting objects based on the active tool selected.
 * @param event Information about the QGraphicsSceneMouseEvent.
 */
void CustomGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF scenePos = event->scenePos();
    qreal width = sceneRect().width()-1;
    qreal height = sceneRect().height()-1;

    if (scenePos.x() < 12.5 || scenePos.x() > width - 12.5)
        return;
    if (scenePos.y() < 12.5 || scenePos.y() > height - 12.5)
        return;

    if (event->button() == Qt::LeftButton)
    {
        switch (this->activeRadio) {
        case 0:
            AddObstacle(scenePos);
            break;
        case 1:
            AddControlledRobot(scenePos);
            break;
        default:
            DeleteObject(scenePos);
            break;
        }
    }
    else
        AddControlledRobot(scenePos);

    QGraphicsScene::mousePressEvent(event);
}

/**
 * @brief Handles mouse move events to dynamically update objects in the scene during a drag operation.
 * @param event Information about the QGraphicsSceneMouseEvent.
 */
void CustomGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        QPointF scenePos = event->scenePos();
        qreal width = sceneRect().width()-1;
        qreal height = sceneRect().height()-1;

        if (scenePos.x() < 12.5 || scenePos.x() > width - 12.5)
            return;
        if (scenePos.y() < 12.5 || scenePos.y() > height - 12.5)
            return;

        switch (this->activeRadio) {
        case 0:
            AddObstacle(scenePos);
            break;
        case 1:
            break;
        default:
            DeleteObject(scenePos);
            break;
        }

        QGraphicsScene::mouseMoveEvent(event);
    }
}

/**
 * @brief Sets up the scene dimensions and creates boundary lines.
 * @param width Width of the scene.
 * @param height Height of the scene.
 */
void CustomGraphicsScene::CreateRoom(int width, int height)
{
    setSceneRect(0, 0, width, height);

    QPen pen(Qt::darkGray);

    this->width = width;
    this->height = height;

    for (auto obstacle : obstacles)
    {
        delete obstacle;
    }

    for (auto robot : robots)
    {
        delete robot;
    }

    obstacles.clear();
    robots.clear();

    clear();

    QPointF topLeft = sceneRect().topLeft();
    QPointF topRight = sceneRect().topRight();
    QPointF bottomLeft = sceneRect().bottomLeft();
    QPointF bottomRight = sceneRect().bottomRight();

    addLine(topLeft.x(), topLeft.y(), topRight.x(), topRight.y(), pen); // Top edge
    addLine(topRight.x(), topRight.y(), bottomRight.x(), bottomRight.y(), pen); // Right edge
    addLine(bottomRight.x(), bottomRight.y(), bottomLeft.x(), bottomLeft.y(), pen); // Bottom edge
    addLine(bottomLeft.x(), bottomLeft.y(), topLeft.x(), topLeft.y(), pen); // Left edge
}

/**
 * @brief Adds an obstacle to the scene at the specified position.
 * @param scenePos The position in the scene where the obstacle is to be placed.
 */
void CustomGraphicsScene::AddObstacle(QPointF scenePos)
{
    if (checkPosition(scenePos) != 0)
        return;

    Obstacle *obstacle = Obstacle::create(scenePos);

    ObjectPainter::PaintObstacle(this, obstacle);
    obstacles.push_back(obstacle);
}

/**
 * @brief Adds a controlled robot to the scene at the specified position, checking for intersections with other robots.
 * @param scenePos The position in the scene where the robot is to be placed.
 */
void CustomGraphicsScene::AddControlledRobot(QPointF scenePos)
{
    Robot *robot = Robot::create(scenePos);

    if (checkPosition(scenePos) != 0)
    {
        QPainterPath robotPath;
        robotPath.addEllipse(robot->boundingRect());
        for (auto r : robots)
        {
            QPainterPath objectPath;
            objectPath.addEllipse(r->boundingRect());

            if (robotPath.intersects(objectPath))
            {
                robot = r;
                robot->turn(30);
                ObjectPainter::PaintRobot(this, robot);
                return;
            }
        }
        return;
    }

    robot = Robot::create(scenePos);
    robots.push_back(robot);
    ObjectPainter::PaintRobot(this, robot);
}

/**
 * @brief Checks if the given position is free from robots or obstacles.
 * @param scenePos The position to check.
 * @return An integer indicating the type of object, if any, at the position.
 */
int CustomGraphicsScene::checkPosition(QPointF scenePos)
{
    QRectF rect = QRectF(scenePos.x()-12.5, scenePos.y()-12.5, 25, 25);
    for (auto robot : robots)
    {
        QPainterPath objectPath;
        objectPath.addEllipse(robot->boundingRect());

        if (rect.intersects(robot->boundingRect()))
            return 1;
    }

    rect = QRectF(scenePos.x(), scenePos.y(), 25, 25);
    for (auto obstacle : obstacles)
    {
        if (rect.intersects(obstacle->boundingRect()))
            return 2;
    }

    return 0;
}

/**
 * @brief Sets the active tool based on user selection from the UI.
 * @param active The index of the selected tool.
 */
void CustomGraphicsScene::SetActive(int active)
{
    activeRadio = active;
}

/**
 * @brief Saves the current scene to a file in CSV format, containing information about all objects within.
 * @param filePath The path to the file where the scene should be saved.
 */
void CustomGraphicsScene::SaveScene(std::string filePath)
{
    std::ofstream file(filePath);

    file << "Type, row, col, angle(robot)" << std::endl;
    file << "ENV," << width << "," << height << std::endl;

    for (const auto& obstacle : obstacles)
    {
        file << "O," << obstacle->getPosition().x()-12.5 << "," << obstacle->getPosition().y()-12.5 << std::endl;
    }

    for (const auto& robot : robots)
    {
        file << "R," << robot->getPosition().x() << "," << robot->getPosition().y() << "," << robot->angle() << std::endl;
    }

    file.close();
    clear();
}

/**
 * @brief Deletes an object from the scene at the specified position, whether it is a robot or an obstacle.
 * @param scenePos The position in the scene where the object to be deleted is located.
 */
void CustomGraphicsScene::DeleteObject(QPointF scenePos)
{
    ObjectPainter::RemoveObject(this, scenePos);

    for (auto r = robots.rbegin(); r != robots.rend(); ++r)
    {
        if ((*r)->boundingRect().contains(scenePos))
        {
            delete *r;
            robots.erase(std::next(r).base());
            return;
        }
    }

    for (auto o = obstacles.rbegin(); o != obstacles.rend(); ++o)
    {
        if ((*o)->boundingRect().contains(scenePos))
        {
            delete *o;
            obstacles.erase(std::next(o).base());
            return;
        }
    }
}

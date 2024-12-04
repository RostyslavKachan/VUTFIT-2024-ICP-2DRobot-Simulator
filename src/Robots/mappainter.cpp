/**
* @file mappainter.cpp
* @brief Implementation of the MapPainter class, that paints the map including boundaries, obstacles, and robots based on the provided environment.
* @author Ondrej Janecka
* @author Rostyslav Kachan
*/

#include "mappainter.h"
#include "objectpainter.h"

/**
 * @brief Constructor for the MapPainter class, initializes a new QGraphicsScene instance.
 * @param parent The parent QObject.
 */
MapPainter::MapPainter(QObject *parent)
    : QGraphicsScene{parent}
{}

/**
 * @brief Paints the entire map including boundaries, obstacles, and robots based on the provided environment.
 * @param environment The environment object containing all the necessary data to paint the map.
 */
void MapPainter::PaintMap(Environment &environment)
{
    // Set the scene size based on the environment dimensions.
    setSceneRect(0, 0, environment.GetSize().x(), environment.GetSize().y());
    
    QPen pen(Qt::darkGray); // Set the pen color for drawing lines.
    clear(); // Clear any existing items in the scene.

    // Draw the borders of the map using lines.
    QPointF topLeft = sceneRect().topLeft();
    QPointF topRight = sceneRect().topRight();
    QPointF bottomLeft = sceneRect().bottomLeft();
    QPointF bottomRight = sceneRect().bottomRight();

    addLine(topLeft.x(), topLeft.y(), topRight.x(), topRight.y(), pen); // Top edge
    addLine(topRight.x(), topRight.y(), bottomRight.x(), bottomRight.y(), pen); // Right edge
    addLine(bottomRight.x(), bottomRight.y(), bottomLeft.x(), bottomLeft.y(), pen); // Bottom edge
    addLine(bottomLeft.x(), bottomLeft.y(), topLeft.x(), topLeft.y(), pen); // Left edge

    // Paint obstacles and robots within the environment.
    paintObstacles(environment);
    paintRobots(environment);
}

/**
 * @brief Paints obstacles within the environment onto the map.
 * @param environment The environment containing obstacles to be painted.
 */
void MapPainter::paintObstacles(Environment &environment)
{
    // Iterate through each obstacle in the environment and paint it on the scene.
    for (auto obstacle : environment.GetObstacles())
    {
        ObjectPainter::PaintObstacle(this, obstacle);
    }
}

/**
 * @brief Paints numbered robots within the environment onto the map.
 * @param environment The environment containing robots to be painted.
 */
void MapPainter::paintRobots(Environment &environment)
{
    int counter = 1;
    for (auto robot : environment.GetRobots())
    {
        ObjectPainter::PaintRobot(this, robot, counter);
        counter++;
    }
}

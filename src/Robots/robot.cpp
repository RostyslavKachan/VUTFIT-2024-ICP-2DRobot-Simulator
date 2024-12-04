/**
* @file robot.cpp
* @brief Implementation of the Robot class that represents a robot in the environment.
* @author Ondrej Janecka
* @author Rostyslav Kachan
*/

#include "robot.h"
#include "qgraphicsscene.h"
#include "qpainter.h"
#include <QtMath>

Robot::Robot(QPointF pos)
{
    this->position = pos;
}

/**
 * @brief Creates a new Robot object at the specified position.
 * 
 * @param pos The position where the Robot should be created.
 * @return A pointer to the newly created Robot object.
 */
Robot* Robot::create(QPointF pos)
{
    Robot *robot = new Robot(pos);

    return robot;
}

/**
 * @brief Get the position of the robot.
 * 
 * @return The position of the robot as a QPointF object.
 */
QPointF Robot::getPosition()
{
    return this->position;
}

/**
 * @brief Turns the robot by a specified angle.
 * 
 * The function updates the direction of the robot by adding the specified angle to the current direction.
 * The angle is wrapped around to ensure it stays within the range of 0 to 359 degrees.
 * 
 * @param angle The angle by which the robot should be turned. Defaults to 30 degrees if not specified.
 */
void Robot::turn(int angle = 30)
{
    direction += angle % 360;
}

/**
 * @brief Returns the angle of the robot.
 *
 * @return The angle of the robot.
 */
int Robot::angle()
{
    return direction;
}

/**
 * @brief Determines whether the robot can move to the next position without colliding with other robots or obstacles.
 * @details The function calculates the next position of the robot based on its current position and direction and uses
 * a triangle with the base set by user and checks for collisions with other robots and obstacles.
 * @param robots A vector of pointers to Robot objects representing other robots in the environment.
 * @param obstacles A vector of pointers to Obstacle objects representing obstacles in the environment.
 * @param size A QPointF object representing the size of the environment.
 * @return True if the robot can move to the next position without collision, false otherwise.
 */
bool Robot::canMove(std::vector<Robot *> &robots, std::vector<Obstacle*> &obstacles, QPointF size)
{
    double nextX = position.x();
    double nextY = position.y();

    double radians = qDegreesToRadians(static_cast<double>(direction));

    nextX += static_cast<int>(moveDistance * qCos(radians));
    nextY -= static_cast<int>(moveDistance * qSin(radians));

    if (nextX + 12.5 >= size.x() || nextX - 12.5 < 0 || nextY + 12.5 >= size.y() || nextY - 12.5 < 0)
        return false;

    // Calculation of triangle points
    double offset = triangleBase / 2.2; // Offset from the center

    // Adjusting the bottom left corner
    double triangleLeftX = nextX + static_cast<int>(triangleBase * qCos(radians)) + static_cast<int>(offset * qCos(radians - M_PI_2));
    double triangleLeftY = nextY - static_cast<int>(triangleBase * qSin(radians)) - static_cast<int>(offset * qSin(radians - M_PI_2));

    // Adjusting the bottom right corner
    double triangleRightX = nextX + static_cast<int>(triangleBase * qCos(radians)) + static_cast<int>(offset * qCos(radians + M_PI_2));
    double triangleRightY = nextY - static_cast<int>(triangleBase * qSin(radians)) - static_cast<int>(offset * qSin(radians + M_PI_2));

    QPointF point1(triangleLeftX, triangleLeftY);  // Bottom left corner
    QPointF point2(triangleRightX, triangleRightY);  // Bottom right corner
    QPointF point3(nextX, nextY); // Vertex

    QPolygonF triangle;
    triangle << point1 << point2 << point3;

    // // Adding the triangle to the scene

    QRectF rect = QRectF(nextX-13, nextY-13, 26, 26);
    QPainterPath robotPath;
    QPainterPath trianglePath;

    robotPath.addEllipse(rect);
    trianglePath.addPolygon(triangle);

    if (triangleLeftX < 0 || triangleLeftX >= size.x() ||
        triangleLeftY < 0 || triangleLeftY >= size.y() ||
        triangleRightX < 0 || triangleRightX >= size.x() ||
        triangleRightY < 0 || triangleRightY >= size.y()) {
        return false; // Collision detected
    }

    for (auto robot : robots)
    {
        if (robot == this)
            continue;
        QPainterPath objectPath;
        objectPath.addEllipse(robot->boundingRect());

        if (objectPath.intersects(robotPath) || objectPath.intersects(trianglePath))
            return false;
    }

    for (auto obstacle : obstacles)
    {
        QPainterPath objectPath;
        objectPath.addRect(obstacle->boundingRect());

        if (objectPath.intersects(robotPath) || objectPath.intersects(trianglePath))
            return false;
    }

    return true;
}

bool Robot::move()
{
    // Výpočet posunu v závislosti na směru
    qreal dx = moveDistance * cos(qDegreesToRadians(static_cast<qreal>(direction)));
    qreal dy = -moveDistance * sin(qDegreesToRadians(static_cast<qreal>(direction)));


    // Aktualizace pozice robota
    position += QPointF(dx, dy);

    return true;
}

QRectF Robot::boundingRect() const
{
    return QRectF(position.x()-12.5, position.y()-12.5, 25, 25);
}

/// @internal
void Robot::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // No need to implement this function
}

/**
 * @brief Checks if the robot is enabled.
 * 
 * @return true if the robot is enabled, false otherwise.
 */
bool Robot::isEnabled()
{
    return enabled;
}

/**
 * @brief Toggles the enabled state of the robot.
 *
 * This function switches the enabled state of the robot. If the robot is currently enabled, it will be disabled,
 * and if it is currently disabled, it will be enabled.
 */
void Robot::switchEnabled()
{
    if (enabled)
        enabled = false;
    else
        enabled = true;
}

/**
 * Retrieves the base of the robot's triangle.
 *
 * @return The base of the robot's triangle.
 */
int Robot::getBase()
{
    return triangleBase;
}

/**
 * @brief Sets the base size of the robot.
 * 
 * This function sets the size of the base of the robot's triangle shape.
 * 
 * @param size The size of the base.
 */
void Robot::setBase(double size)
{
    triangleBase = size;
}

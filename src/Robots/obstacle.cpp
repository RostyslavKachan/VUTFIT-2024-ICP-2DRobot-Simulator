/**
* @file obstacle.cpp
* @brief Implementation of the Obstacle class that represents an obstacle in the environment.
* @author Ondrej Janecka
* @author Rostyslav Kachan
*/

#include "obstacle.h"
#include "qpainter.h"

/**
 * @brief Constructor for the Obstacle class, initializes an obstacle at a given position.
 * @param pos QPointF indicating the position of the obstacle.
 */
Obstacle::Obstacle(QPointF pos)
{
    this->position = pos; // Set the internal position state to the provided value.
}

/**
 * @brief Getter method to retrieve the position of the obstacle.
 * @return Returns the current position of the obstacle as a QPointF.
 */
QPointF Obstacle::getPosition()
{
    return position; // Return the stored position.
}

/**
 * @brief Static factory method to create and return a new obstacle at a given position.
 * @param pos QPointF indicating the position where the obstacle should be created.
 * @return Pointer to the newly created Obstacle object.
 */
Obstacle* Obstacle::create(QPointF pos)
{
    Obstacle *obstacle = new Obstacle(pos); // Create a new Obstacle instance at the specified position.
    return obstacle; // Return the new obstacle.
}

/**
 * @brief Provides the bounding rectangle of the obstacle, which defines the area the obstacle occupies in the scene.
 * @return QRectF defining the bounding rectangle of the obstacle.
 */
QRectF Obstacle::boundingRect() const
{
    return QRectF(position.x(), position.y(), 25, 25); // Return a QRectF object initialized with the position and dimensions of the obstacle.
}

/// @internal
void Obstacle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // No need to implement this method.
}

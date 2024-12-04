/**
* @file objectpainter.cpp
* @brief Implementation of the ObjectPainter class, that paints robots and obstacles on the scene based on the provided objects and positions.
* @author Ondrej Janecka
* @author Rostyslav Kachan
*/

#include "objectpainter.h"
#include "qgraphicsitem.h"
#include "mappainter.h"
#include <QtMath>   

/**
 * @brief Paints a representation of a robot on the scene.
 * 
 * This function paints a representation of a robot with a number and triangle representing the robot's field of view on the provided scene.
 * The robot is painted as a circle with a conical gradient fill, where the gradient differs based on the robot's angle.
 * 
 * @param scene Pointer to the CustomGraphicsScene where the robot will be painted.
 * @param robot Pointer to the Robot object to be painted.
 */
void ObjectPainter::PaintRobot(CustomGraphicsScene *scene, Robot *robot)
{
    // Determining the smaller of the width and height values of the cell for the circle diameter
    qreal radius = 12.5;
    QPointF position = robot->getPosition();

    // Creating a circle that fits into the cell
    QPointF circleCenter(position.x(), position.y());
    QRectF boundingRect(circleCenter.x() - radius, circleCenter.y() - radius, 2 * radius, 2 * radius);
    QGraphicsEllipseItem *circle = new QGraphicsEllipseItem(boundingRect);

    int angle = robot->angle();
    QConicalGradient gradient(boundingRect.center(), angle);
    gradient.setColorAt(0, Qt::white); // Start color
    gradient.setColorAt(1, Qt::blue); // End color
    QBrush brush(gradient);
    brush.setStyle(Qt::ConicalGradientPattern);
    circle->setBrush(brush);

    scene->addItem(circle);
}

/**
 * @brief Paints a representation of a robot on the scene.
 * 
 * This function paints a representation of a robot with a number and triangle representing the robot's field of view on the provided scene.
 * 
 * @param scene Pointer to the MapPainter where the robot will be painted.
 * @param robot Pointer to the Robot object to be painted.
 * @param num The number associated with the robot.
 */
void ObjectPainter::PaintRobot(MapPainter *scene, Robot *robot, int num)
{
    qreal radius = 12.5;
    QPointF position = robot->getPosition();
    qreal smallRadius = 3.0;
    qreal angleInRadians = qDegreesToRadians(static_cast<float>(robot->angle()));
    qreal eyeDistance = 2; // Distance of eyes from the center of the robot

    // Creating the main circle representing the robot
    QGraphicsEllipseItem *circle = new QGraphicsEllipseItem(position.x() - radius, position.y() - radius, 2 * radius, 2 * radius);
    circle->setBrush(Qt::white);

    // Creating the first small "eye"
    qreal firstEyeCenterX = position.x() + (radius - eyeDistance) * cos(angleInRadians - qDegreesToRadians(static_cast<double>(25)));
    qreal firstEyeCenterY = position.y() - (radius - eyeDistance) * sin(angleInRadians - qDegreesToRadians(static_cast<double>(25)));
    QGraphicsEllipseItem *firstEye = new QGraphicsEllipseItem(firstEyeCenterX - smallRadius, firstEyeCenterY - smallRadius, 2 * smallRadius, 2 * smallRadius);
    firstEye->setBrush(Qt::red);

    // Creating the second small "eye"
    qreal secondEyeCenterX = position.x() + (radius - eyeDistance) * cos(angleInRadians + qDegreesToRadians(static_cast<double>(25)));
    qreal secondEyeCenterY = position.y() - (radius - eyeDistance) * sin(angleInRadians + qDegreesToRadians(static_cast<double>(25)));
    QGraphicsEllipseItem *secondEye = new QGraphicsEllipseItem(secondEyeCenterX - smallRadius, secondEyeCenterY - smallRadius, 2 * smallRadius, 2 * smallRadius);
    secondEye->setBrush(Qt::red);

    // Creating a text item with the number
    QGraphicsTextItem *textItem = new QGraphicsTextItem(QString::number(num));
    textItem->setFont(QFont("Arial", 16, QFont::Bold));
    textItem->setDefaultTextColor(Qt::black);
    textItem->setPos(position.x() - textItem->boundingRect().width() / 2.0, position.y() - textItem->boundingRect().height() / 2.0);

    // Adding the main circle, eyes, and text item to the scene

    int nextX = position.x();
    int nextY = position.y();

    QPointF nextCenter = QPointF(nextX, nextY);

    // Calculation of triangle points
    int triangleBase = robot->getBase(); // Length of one arm of the triangle
    int offset = triangleBase / 2.2; // Offset from the center

    // Adjusting the bottom left corner
    int triangleLeftX = nextX + static_cast<int>(triangleBase * qCos(angleInRadians)) + static_cast<int>(offset * qCos(angleInRadians - M_PI_2));
    int triangleLeftY = nextY - static_cast<int>(triangleBase * qSin(angleInRadians)) - static_cast<int>(offset * qSin(angleInRadians - M_PI_2));

    // Adjusting the bottom right corner
    int triangleRightX = nextX + static_cast<int>(triangleBase * qCos(angleInRadians)) + static_cast<int>(offset * qCos(angleInRadians + M_PI_2));
    int triangleRightY = nextY - static_cast<int>(triangleBase * qSin(angleInRadians)) - static_cast<int>(offset * qSin(angleInRadians + M_PI_2));

    QPointF point1(triangleLeftX, triangleLeftY);  // Bottom left corner
    QPointF point2(triangleRightX, triangleRightY);  // Bottom right corner
    QPointF point3(nextCenter); // Vertex

    QPolygonF triangle;
    triangle << point1 << point2 << point3;
    QGraphicsPolygonItem *polygonItem = new QGraphicsPolygonItem(triangle);
    QPen pen(Qt::yellow);
    polygonItem->setPen(pen);

    scene->addItem(polygonItem);
    scene->addItem(circle);
    scene->addItem(firstEye);
    scene->addItem(secondEye);
    scene->addItem(textItem);
}

/**
 * @brief Paints an obstacle on the scene.
 * 
 * This function creates a rectangle representing the obstacle and adds it to the scene with a diagonal cross pattern brush.
 * 
 * @param scene The CustomGraphicsScene where the obstacle will be painted.
 * @param obstacle The obstacle to be painted.
 */
void ObjectPainter::PaintObstacle(CustomGraphicsScene *scene, Obstacle *obstacle)
{
    QRectF rect(obstacle->getPosition().x()-12.5, obstacle->getPosition().y()-12.5, 25, 25);
    QBrush brush(Qt::lightGray);
    brush.setStyle(Qt::DiagCrossPattern);
    scene->addRect(rect, QPen(), brush);
}

/**
 * @brief Paints an obstacle on the scene.
 * 
 * This function creates a rectangle representing the obstacle and adds it to the scene
 * with a diagonal cross pattern brush.
 * 
 * @param scene A pointer to the MapPainter scene.
 * @param obstacle A pointer to the obstacle to be painted.
 */
void ObjectPainter::PaintObstacle(MapPainter *scene, Obstacle *obstacle)
{
    QRectF rect(obstacle->getPosition().x(), obstacle->getPosition().y(), 25, 25);
    QBrush brush(Qt::lightGray);
    brush.setStyle(Qt::DiagCrossPattern);
    scene->addRect(rect, QPen(), brush);
}

/**
 * @brief Removes objects from the scene at the specified position.
 * 
 * This function removes all objects that intersect with the given position
 * from the specified scene. It iterates through all items at the position
 * and checks if they are QGraphicsLineItems. If an item is not a line item
 * and it intersects with the position, it is removed from the scene and
 * its memory is deallocated.
 * 
 * @param scene The scene from which to remove objects.
 * @param scenePos The position in the scene where objects should be removed.
 */
void ObjectPainter::RemoveObject(CustomGraphicsScene *scene, QPointF scenePos)
{
    QList<QGraphicsItem*> itemsAtPos = scene->items(scenePos);

    for (QGraphicsItem* item : itemsAtPos)
    {
        QGraphicsLineItem* lineItem = qgraphicsitem_cast<QGraphicsLineItem*>(item);
        if (lineItem)
        {
            continue;
        }

        if (item->contains(item->mapFromScene(scenePos)))
        {
            scene->removeItem(item);
            delete item; // Uvolnění paměti alokované pro položku
        }
    }
}

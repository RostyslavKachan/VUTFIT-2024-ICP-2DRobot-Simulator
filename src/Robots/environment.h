/**
* @file environment.h
* @author Ondrej Janecka
* @author Rostyslav Kachan
*/

#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "obstacle.h"
#include "robot.h"
#include <fstream>
#include <vector>

class Environment
{
private:
    QPointF size;
    Robot *controlledRobot;
    std::vector<Robot*> robots;
    std::vector<Obstacle*> obstacles;
    bool checkPosition(QPointF pos);

public:
    Environment(QPointF size);
    bool CreateObstacle(QPointF pos);
    
    
    static Environment* LoadEnvironment(std::ifstream& file);
    bool LoadObjects(std::ifstream& file);
    std::vector<Robot*>& GetRobots();
    std::vector<Obstacle*>& GetObstacles();
    int GetRows();
    int GetCols();
    bool ContainsPosition(QPointF pos);
    Robot& GetControlledRobot();
    Robot& GetRobotByNumber(int number);

    void SetControlledRobot(int number);
    QPointF GetSize();

    ~Environment();
};

#endif // ENVIRONMENT_H

/**
* @file environment.cpp
* @brief Class for the Environment object that contains robots and obstacles.
* @author Ondrej Janecka
* @author Rostyslav Kachan
*/

#include "environment.h"
#include "qdebug.h"
#include "qlogging.h"
#include <fstream>
#include <string>
#include <sstream>

/**
 * @brief Constructor for the Environment class, initializes with a specified size.
 * @param size QPointF representing the size of the environment.
 */
Environment::Environment(QPointF size)
{
    this->size = size;
    controlledRobot = nullptr;
}

/**
 * @brief Destructor for the Environment class, deallocates obstacles and robots.
 */
Environment::~Environment()
{
    for (auto obstacle : obstacles)
    {
        delete obstacle;
    }

    for (auto robot : robots)
    {
        delete robot;
    }
}

/**
 * @brief Creates an obstacle at a specified position.
 * @param pos QPointF where the obstacle is to be created.
 * @return Returns true if the obstacle is created successfully.
 */
bool Environment::CreateObstacle(QPointF pos)
{
    Obstacle *obstacle = Obstacle::create(pos);
    obstacles.push_back(obstacle);
    return true;
}

/**
 * @brief Loads the environment from a file stream.
 * @param file Reference to an ifstream from which the environment is loaded.
 * @return Pointer to the loaded Environment object, or nullptr if loading fails.
 */
Environment* Environment::LoadEnvironment(std::ifstream& file)
{
    Environment *environment = nullptr;

    if (!file.is_open())
    {
        return nullptr;
    }

    std::string line;
    std::getline(file, line); // Skip the first line

    if (std::getline(file, line))
    {
        if (line.find("ENV") == 0)
        {
            std::stringstream ss(line);
            std::string token;
            std::vector<std::string> tokens;
            while (std::getline(ss, token, ','))
            {
                tokens.push_back(token);
            }

            if (tokens.size() >= 3)
            {
                std::string x = tokens[1];
                std::string y = tokens[2];

                environment = new Environment(QPointF(std::stod(x), std::stod(y)));
            }
            else
                return environment;
        }
        else
            return environment;
    }
    else
        return environment;

    return environment;
}

/**
 * @brief Loads objects (obstacles or robots) into the environment from a file stream.
 * @param file Reference to an ifstream from which the objects are loaded.
 * @return Returns true if all objects are loaded successfully.
 */
bool Environment::LoadObjects(std::ifstream& file)
{
    std::string line;

    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string token;
        std::vector<std::string> tokens;
        int x, y;
        while (std::getline(ss, token, ','))
        {
            tokens.push_back(token);
        }

        if (tokens.size() >= 3)
        {
            x = std::stoi(tokens[1]);
            y = std::stoi(tokens[2]);
        }
        else
            return false;

        if (tokens[0] == "O" && tokens.size() == 3)
        {
            Obstacle *obstacle = Obstacle::create(QPointF(x, y));
            obstacles.push_back(obstacle);
        }
        else if (tokens[0] == "R" && tokens.size() == 4)
        {
            Robot *robot = Robot::create(QPointF(x, y));
            robot->turn(std::stoi(tokens[3]) / 45);
            robots.push_back(robot);
        }
        else
            return false;
    }

    return true;
}

/**
 * @brief Retrieves the vector of robots in the environment.
 * @return Reference to the vector of robot pointers.
 */
std::vector<Robot*> &Environment::GetRobots()
{
    return robots;
}

/**
 * @brief Retrieves the vector of obstacles in the environment.
 * @return Reference to the vector of obstacle pointers.
 */
std::vector<Obstacle*> &Environment::GetObstacles()
{
    return obstacles;
}

/**
 * @brief Gets the number of rows in the environment.
 * @return Number of rows.
 */
int Environment::GetRows()
{
    return size.x();
}

/**
 * @brief Gets the number of columns in the environment.
 * @return Number of columns.
 */
int Environment::GetCols()
{
    return size.y();
}

/**
 * @brief Gets the size of the environment.
 * @return QPointF representing the size.
 */
QPointF Environment::GetSize()
{
    return size;
}

/**
 * @brief Retrieves the controlled robot in the environment.
 * @return Reference to the controlled Robot object.
 */
Robot& Environment::GetControlledRobot()
{
    return *controlledRobot;
}

/**
 * @brief Sets the controlled robot in the environment based on the specified index.
 * @param number Index of the robot to be controlled. If zero, controlled robot is set to nullptr.
 */
void Environment::SetControlledRobot(int number)
{
    if (number == 0)
        controlledRobot = nullptr;
    else
        controlledRobot = robots[number-1];
}

/**
 * @brief Retrieves a robot by its index number in the list.
 * @param number Index of the robot to retrieve.
 * @return Reference to the Robot object.
 */
Robot& Environment::GetRobotByNumber(int number)
{
    return *robots[number-1];
}

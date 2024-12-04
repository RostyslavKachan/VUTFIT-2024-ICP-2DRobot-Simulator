/**
* @file simulationwidget.cpp
* @brief Widget for simulating the movement of robots in the environment.
* @author Ondrej Janecka
* @author Rostyslav Kachan
*/

#include "simulationwidget.h"

/**
 * @brief Constructs a SimulationWidget object.
 * 
 * This constructor initializes the SimulationWidget with the given parent widget.
 * It also creates the UI, sets up the scene, and connects various signals and slots.
 * Additionally, it prompts the user to select a CSV file for the map and loads the map.
 * Finally, it sets the seed for the random number generator.
 * 
 * @param parent The parent widget.
 */
SimulationWidget::SimulationWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SimulationWidget)
    , scene(new MapPainter(parent))
    , environment(nullptr)
{
    ui->setupUi(this);
    scene = new MapPainter(parent);
    ui->graphicsView->setScene(scene);

    connect(ui->backButton, SIGNAL(clicked(bool)), this, SLOT(backButton_clicked()));
    connect(ui->ppButton, SIGNAL(clicked(bool)), this, SLOT(ppButton_clicked()));
    connect(ui->forwardButton, SIGNAL(clicked(bool)), this, SLOT(forwardMove()));
    connect(ui->leftButton, SIGNAL(clicked(bool)), this, SLOT(leftRotate()));
    connect(ui->rightButton, SIGNAL(clicked(bool)), this, SLOT(rightRotate()));
    connect(ui->multiplySpin, SIGNAL(valueChanged(double)), this, SLOT(multiplySpin_valueChanged()));
    connect(ui->reloadButton, SIGNAL(clicked(bool)), this, SLOT(reloadButton_clicked()));

    this->mapFilePath = QFileDialog::getOpenFileName(this, tr("Open CSV File"), "", tr("CSV Files (*.csv);;All Files (*)"));

    loadMap();

    // Set the seed for the random number generator based on the memory address of the object
    srand(reinterpret_cast<std::uintptr_t>(this));
}

/**
 * @brief Destructor for the SimulationWidget class.
 * 
 * This destructor is responsible for cleaning up the memory allocated for the UI object.
 * It deletes the UI object created by the constructor.
 */
SimulationWidget::~SimulationWidget()
{
    delete ui;
}

/**
 * @brief Slot function triggered when the back button is clicked.
 * 
 * This function emits the `backRequested` signal, indicating that the user has requested to go back.
 * 
 * @see backRequested()
 */
void SimulationWidget::backButton_clicked()
{
    emit backRequested();
}

/**
 * @brief Loads the map from the selected CSV file and creates the map.
 * 
 * Creates dynamicly UI elements for each robot in the environment.
 */
void SimulationWidget::loadMap()
{
    QString filePath = this->mapFilePath;

    if (filePath.isEmpty())
    {
        QMessageBox::information(this, tr("Error"), tr("Empty file path."));
        emit backRequested();
        return;
    }

    std::ifstream file(filePath.toStdString());

    environment = Environment::LoadEnvironment(file);

    if (environment == nullptr)
    {
        file.close();
        emit backRequested();
        return;
    }

    if (!environment->LoadObjects(file))
    {
        file.close();
        emit backRequested();
        return;
    }

    file.close();

    this->mapFilePath = filePath;

    scene->PaintMap(*environment);

    QWidget *widget = new QWidget;
    QHBoxLayout *layout = new QHBoxLayout(widget);
    QButtonGroup *buttonGroup = new QButtonGroup(this);
    QRadioButton *noneRadioButton = new QRadioButton(QString("None"));
    noneRadioButton->setChecked(true);

    layout->setAlignment(Qt::AlignTop);

    layout->addWidget(noneRadioButton);
    widget->setLayout(layout);

    ui->robotWidget->layout()->addWidget(widget);
    buttonGroup->addButton(noneRadioButton);

    for (int i = 1; i <= environment->GetRobots().size(); i++) {
        QWidget *widget = new QWidget;
        QVBoxLayout *layout = new QVBoxLayout(widget);
        QHBoxLayout *buttonLayout = new QHBoxLayout;
        QPushButton *startStopButton = new QPushButton("Stop");
        QRadioButton *robotRadioButton = new QRadioButton(QString("Robot %1").arg(i));
        QSlider *triangleBaseSlider = new QSlider(Qt::Horizontal);

        triangleBaseSlider->setRange(0, 300);
        triangleBaseSlider->setValue(30);

        layout->setAlignment(Qt::AlignTop);

        buttonLayout->addWidget(robotRadioButton);
        buttonLayout->addWidget(startStopButton);

        layout->addLayout(buttonLayout);

        layout->addWidget(triangleBaseSlider);

        widget->setLayout(layout);

        ui->robotWidget->layout()->addWidget(widget);

        buttonGroup->addButton(robotRadioButton);

        // Connect signals and slots for the robot triangle base slider 
        connect(triangleBaseSlider, &QSlider::valueChanged, this, [=](int value) {
            environment->GetRobotByNumber(i).setBase(value);
            scene->PaintMap(*environment);
        });

        // Connect signals and slots for the start/stop button
        connect(startStopButton, &QPushButton::clicked, this, [=]() {
            environment->GetRobotByNumber(i).switchEnabled();

            if (environment->GetRobotByNumber(i).isEnabled())
                startStopButton->setText("Stop");
            else
                startStopButton->setText("Start");
        });
    }

    // Add a signal to the button group to detect when a radio button is clicked
    connect(buttonGroup, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(robotPicker(QAbstractButton*)));

    // Set up the simulation timer
    simulationTimer = new QTimer(this);
    connect(simulationTimer, &QTimer::timeout, this, &SimulationWidget::simulate);
}

/**
 * @brief Sets the controlled robot based on the selected radio button.
 * 
 * This function is called when a radio button is clicked in the simulation widget.
 * It retrieves the text of the selected radio button and determines the robot number
 * based on the text. If the selected radio button is "None", the controlled robot is set to 0.
 * Otherwise, the text is split into parts and the last part is converted to an integer to get
 * the robot number. The controlled robot is then set to the determined robot number.
 * 
 * @param button A pointer to the selected radio button.
 */
void SimulationWidget::robotPicker(QAbstractButton *button) {
    QRadioButton *selectedRadioButton = qobject_cast<QRadioButton*>(button);
    if (selectedRadioButton && selectedRadioButton->isChecked()) 
    {
        QString robotName = selectedRadioButton->text();
        if (robotName == "None")
            environment->SetControlledRobot(0);
        else 
        {
            QStringList parts = robotName.split(" ");
            int robotNumber = parts.back().toInt();
            environment->SetControlledRobot(robotNumber);
        }
    }
}

/**
 * @brief Handles the click event of the "ppButton" button.
 * 
 * This function is called when the "ppButton" button is clicked. It toggles the simulation state
 * between running and paused. If the simulation is currently running, it stops the simulation and
 * the associated timer. If the simulation is paused, it starts the simulation timer with a delay
 * based on the value of the "multiplySpin" spin box.
 */
void SimulationWidget::ppButton_clicked()
{
    if (simulationRunning)
    {
        simulationRunning = false;
        simulationTimer->stop();
    }
    else
    {
        simulationRunning = true;
        simulationTimer->start(50 / ui->multiplySpin->value());
    }
}

/**
 * @brief Simulates the movement of robots in the environment.
 *
 * This function iterates through each robot in the environment and checks if it is enabled and not the controlled robot.
 * If the robot can move without colliding with other robots or obstacles, it moves in a straight line.
 * Otherwise, it randomly turns the robot.
 *
 * After simulating the movement of all robots, the scene is updated to reflect the changes in the environment.
 */
void SimulationWidget::simulate()
{
    for (auto robot : environment->GetRobots())
    {
        if (!robot->isEnabled() || robot == &environment->GetControlledRobot())
            continue;
        if(robot->canMove(environment->GetRobots(), environment->GetObstacles(), environment->GetSize()))
            robot->move();
        else
            robot->turn(rand()%360 + 1);

    }
    scene->PaintMap(*environment);
}

/**
 * @brief Moves the controlled robot forward in the simulation.
 *
 */
void SimulationWidget::forwardMove()
{
    Robot* robot = &environment->GetControlledRobot();

    if (robot == nullptr || !robot->canMove(environment->GetRobots(), environment->GetObstacles(), environment->GetSize()))
        return;

    robot->move();

    scene->PaintMap(*environment);
}

/**
 * @brief Rotate the controlled robot left in the simulation.
 *
 */
void SimulationWidget::leftRotate()
{
    Robot* robot = &environment->GetControlledRobot();

    if (robot == nullptr)
        return;

    robot->turn(10);

    scene->PaintMap(*environment);
}

/**
 * @brief Rotate the controlled robot right in the simulation.
 *
 */
void SimulationWidget::rightRotate()
{
    Robot* robot = &environment->GetControlledRobot();

    if (robot == nullptr)
        return;

    robot->turn(-10);

    scene->PaintMap(*environment);
}

/**
 * @brief Handles key press events for the SimulationWidget.
 * 
 * This function is called when a key is pressed while the SimulationWidget has focus.
 * It checks the key code and performs the corresponding action based on the key pressed.
 * 
 * @param event A pointer to the QKeyEvent object that contains information about the key event.
 */
void SimulationWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_W || event->key() == Qt::Key_Up)
        forwardMove();
    else if (event->key() == Qt::Key_A || event->key() == Qt::Key_Left)
        leftRotate();
    else if (event->key() == Qt::Key_D || event->key() == Qt::Key_Right)
        rightRotate();

    QWidget::keyPressEvent(event);
}

/**
 * @brief This slot is called when the value of the multiplySpin widget changes.
 *        It adjusts the simulation timer interval based on the new value.
 *        If the simulation is currently running, it restarts the timer with the updated interval.
 */
void SimulationWidget::multiplySpin_valueChanged()
{
    if (simulationRunning)
        simulationTimer->start(100 / ui->multiplySpin->value());
}

/**
 * @brief Slot function triggered when the reload button is clicked in the SimulationWidget.
 * 
 * Seeds the random number generator with the memory address of the current object again.
 * Stops the simulation timer and sets the simulationRunning flag to false.
 * Load the map from the file again and create map.
 */
void SimulationWidget::reloadButton_clicked()
{
    srand(reinterpret_cast<std::uintptr_t>(this));

    simulationRunning = false;
    simulationTimer->stop();

    QString filePath = this->mapFilePath;

    if (filePath.isEmpty())
    {
        emit backRequested();
        return;
    }

    std::ifstream file(filePath.toStdString());

    environment = Environment::LoadEnvironment(file);

    if (environment == nullptr)
    {
        file.close();
        emit backRequested();
        return;
    }

    if (!environment->LoadObjects(file))
    {
        file.close();
        emit backRequested();
        return;
    }

    file.close();

    scene->PaintMap(*environment);
}

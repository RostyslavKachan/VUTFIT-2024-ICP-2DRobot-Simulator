/**
* @file mainwindow.cpp
* @brief Implementation of the MainWindow class and connecting navigation signals to slots.
* @author Ondrej Janecka
* @author Rostyslav Kachan
*/

#include "mainwindow.h"
#include "creatorwidget.h"
#include "simulationwidget.h"
#include "ui_mainwindow.h"
#include "welcomewidget.h"

/**
 * @brief Constructor for the MainWindow class, initializes user interface components and displays the welcome widget.
 * @param parent The parent widget, usually null for main application windows.
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this); // Set up the UI components based on a predefined layout in the UI designer.
    ShowWelcome(); // Show the welcome screen initially.
}

/**
 * @brief Destructor for the MainWindow class, cleans up the UI components.
 */
MainWindow::~MainWindow()
{
    delete ui; // Safely delete the user interface to free resources.
}

/**
 * @brief Displays the welcome widget in the main window.
 * 
 * This function initializes the welcome widget, sets it as the central widget of the window, 
 * and connects signals from the welcome widget to appropriate slots for handling user actions.
 */
void MainWindow::ShowWelcome()
{
    WelcomeWidget *welcomeWidget = new WelcomeWidget(this); // Create a new welcome widget.

    // Connect signals from the welcome widget to slots in MainWindow for processing user actions.
    connect(welcomeWidget, &WelcomeWidget::loadRequested, this, &MainWindow::ShowSimulation);
    connect(welcomeWidget, &WelcomeWidget::createRequested, this, &MainWindow::ShowCreator);

    setCentralWidget(welcomeWidget); // Set the welcome widget as the central widget.
    update(); // Refresh the MainWindow to display the welcome widget.
}

/**
 * @brief Displays the creator widget, where users can create new environments or objects.
 * 
 * This function initializes the creator widget, sets it as the central widget of the window, 
 * and ensures that there is a way to navigate back to the welcome screen.
 */
void MainWindow::ShowCreator()
{
    CreatorWidget *creatorWidget = new CreatorWidget(this); // Create a new creator widget.

    // Connect the backRequested signal from the creator widget to the ShowWelcome slot to handle navigation.
    connect(creatorWidget, &CreatorWidget::backRequested, this, &MainWindow::ShowWelcome);

    setCentralWidget(creatorWidget); // Set the creator widget as the central widget.
    update(); // Update the MainWindow to display the creator widget.
}

/**
 * @brief Displays the simulation widget, where users can simulate and interact with environments.
 * 
 * This function initializes the simulation widget, sets it as the central widget of the window,
 * and connects a signal to navigate back to the welcome screen upon user request.
 */
void MainWindow::ShowSimulation()
{
    SimulationWidget *simulationWidget = new SimulationWidget(this); // Create a new simulation widget.

    // Connect the backRequested signal from the simulation widget to the ShowWelcome slot for navigation.
    connect(simulationWidget, &SimulationWidget::backRequested, this, &MainWindow::ShowWelcome);

    setCentralWidget(simulationWidget); // Set the simulation widget as the central widget.
    update(); // Refresh the MainWindow to display the simulation widget.
}

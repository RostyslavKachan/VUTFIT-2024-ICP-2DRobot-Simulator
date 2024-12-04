/**
* @file welcomewidget.cpp
* @brief Implementation of the WelcomeWidget class and its associated slots for handling user actions in the welcome screen of the application. 
* @details This widget is the first screen shown to the user when the application starts, and it provides options to load an existing environment or create a new one.
* @author Ondrej Janecka
* @author Rostyslav Kachan
*/

#include "welcomewidget.h"
#include "ui_welcomewidget.h"

/**
 * @brief Constructor for the WelcomeWidget class, initializes the user interface and sets up connections for UI elements.
 * @param parent The parent widget, typically the main application window.
 */
WelcomeWidget::WelcomeWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WelcomeWidget)
{
    ui->setupUi(this); // Initialize the user interface from the designed form.

    // Connect the load and create buttons to their respective slots.
    connect(ui->loadButton, SIGNAL(clicked(bool)), this, SLOT(loadButton_clicked()));
    connect(ui->createButton, SIGNAL(clicked(bool)), this, SLOT(createButton_clicked()));
}

/**
 * @brief Destructor for the WelcomeWidget class, cleans up the user interface resources.
 */
WelcomeWidget::~WelcomeWidget()
{
    delete ui; // Properly delete the user interface to free resources.
}

/**
 * @brief Slot connected to the load button's clicked signal. Emits the loadRequested signal when the button is clicked.
 */
void WelcomeWidget::loadButton_clicked()
{
    emit loadRequested(); // Notify that a load operation has been requested.
}

/**
 * @brief Slot connected to the create button's clicked signal. Emits the createRequested signal when the button is clicked.
 */
void WelcomeWidget::createButton_clicked()
{
    emit createRequested(); // Notify that a create operation has been requested.
}

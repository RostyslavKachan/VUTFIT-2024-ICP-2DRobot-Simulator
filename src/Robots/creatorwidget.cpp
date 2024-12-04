/**
* @file creatorwidget.cpp
* @brief Implementation of the CreatorWidget class.
* @details Implementation of the CreatorWidget class, which is a widget for creating a new room and saving it to a file.
* @author Ondrej Janecka
* @author Rostyslav Kachan
*/

#include "creatorwidget.h"

/**
 * @brief Constructor for CreatorWidget, initializes components and connects signals.
 * 
 * @param parent The parent widget of this widget, typically a main window.
 */
CreatorWidget::CreatorWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CreatorWidget)
    , scene(new CustomGraphicsScene(parent))
{
    ui->setupUi(this);
    scene = new CustomGraphicsScene(parent);
    ui->graphicsView->setScene(scene);
    ui->widthLine->setFocus();

    connect(ui->setSizeButton, SIGNAL(clicked(bool)), this, SLOT(setSize()));
    connect(ui->widthLine, SIGNAL(editingFinished()), this, SLOT(widthLine_finished()));
    connect(ui->heightLine, SIGNAL(editingFinished()), this, SLOT(setSize()));

    connect(ui->saveButton, SIGNAL(clicked(bool)), this, SLOT(saveButton_clicked()));
    connect(ui->cancelButton, SIGNAL(clicked(bool)), this, SLOT(cancelButton_clicked()));

    connect(ui->obstacleRadio, SIGNAL(toggled(bool)), this, SLOT(radio_toggled()));
    connect(ui->robotRadio, SIGNAL(toggled(bool)), this, SLOT(radio_toggled()));
    connect(ui->deleteRadio, SIGNAL(toggled(bool)), this, SLOT(radio_toggled()));
}

/**
 * @brief Destructor for CreatorWidget, cleans up UI resources.
 */
CreatorWidget::~CreatorWidget()
{
    delete ui;
}

/**
 * @brief Sets the dimensions of the room in the scene based on user input from UI.
 */
void CreatorWidget::setSize()
{
    scene->CreateRoom(ui->widthLine->text().toInt(), ui->heightLine->text().toInt());
    ui->heightLine->clearFocus();
}

/**
 * @brief Handles the save button click event; opens a dialog for saving the file and saves the scene.
 */
void CreatorWidget::saveButton_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save CSV File"), "", tr("CSV Files (*.csv);;All Files (*)"));
    if (filePath.isEmpty())
        return;

    scene->SaveScene(filePath.toStdString());
    QMessageBox::information(this, tr("Success"), tr("File saved."));
    emit backRequested();
}

/**
 * @brief Handles the cancel button click event; emits a signal to request returning to the previous view.
 */
void CreatorWidget::cancelButton_clicked()
{
    emit backRequested();
}

/**
 * @brief Toggles the active tool based on which radio button is checked.
 */
void CreatorWidget::radio_toggled()
{
    if (ui->obstacleRadio->isChecked())
        scene->SetActive(0);
    else if (ui->robotRadio->isChecked())
        scene->SetActive(1);
    else
        scene->SetActive(2);
}

/**
 * @brief Finishes editing in widthLine and shifts focus to heightLine.
 */
void CreatorWidget::widthLine_finished()
{
    ui->heightLine->setFocus();
}

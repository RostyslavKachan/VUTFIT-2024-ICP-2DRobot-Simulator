/**
* @file simulationwidget.h
* @author Ondrej Janecka
* @author Rostyslav Kachan
*/

#ifndef SIMULATIONWIDGET_H
#define SIMULATIONWIDGET_H

#include "environment.h"
#include "mappainter.h"
#include <QWidget>
#include <QAbstractButton>
#include "mappainter.h"
#include "qobjectdefs.h"
#include "qradiobutton.h"
#include "ui_simulationwidget.h"
#include <QFileDialog>
#include <QButtonGroup>
#include <QKeyEvent>
#include <QTimer>
#include <QSlider>
#include <QMessageBox>
#include <QAbstractButton>

namespace Ui {
class SimulationWidget;
}

class SimulationWidget : public QWidget
{
    Q_OBJECT

protected:
    void keyPressEvent(QKeyEvent *event) override;

public:
    explicit SimulationWidget(QWidget *parent = nullptr);
    ~SimulationWidget();

private slots:
    void backButton_clicked();
    void ppButton_clicked();
    void forwardMove();
    void leftRotate();
    void rightRotate();
    void multiplySpin_valueChanged();
    void reloadButton_clicked();
    void robotPicker(QAbstractButton*);

Q_SIGNALS:
    void backRequested();

private:
    Ui::SimulationWidget *ui;
    MapPainter *scene;
    void loadMap();
    void parseFile(std::string filePath);
    Environment *environment;
    void simulate();
    QTimer *simulationTimer;
    bool simulationRunning = false;
    QString mapFilePath;
};

#endif // SIMULATIONWIDGET_H

/**
* @file mainwindow.h
* @author Ondrej Janecka
* @author Rostyslav Kachan
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void ShowWelcome();
    void ShowCreator();
    void ShowSimulation();
};

#endif // MAINWINDOW_H

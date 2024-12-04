/**
* @file creatorwidget.h
* @brief Header file for the CreatorWidget class.
* @author Ondrej Janecka
* @author Rostyslav Kachan
*/

#ifndef CREATORWIDGET_H
#define CREATORWIDGET_H

#include "customgraphicsscene.h"
#include "ui_creatorwidget.h"
#include <QWidget>
#include <QFileDialog>
#include <QMessageBox>

namespace Ui {
class CreatorWidget;
}

class CreatorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CreatorWidget(QWidget *parent = nullptr);
    ~CreatorWidget();
    static int activeRadio;
    int getActive();

private slots:
    void setSize();
    void radio_toggled();
    void saveButton_clicked();
    void cancelButton_clicked();
    void widthLine_finished();

Q_SIGNALS:
    void backRequested();

private:
    Ui::CreatorWidget *ui;
    CustomGraphicsScene *scene;
};

#endif // CREATORWIDGET_H

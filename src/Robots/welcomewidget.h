/**
* @file welcomewidget.h
* @author Ondrej Janecka
* @author Rostyslav Kachan
*/

#ifndef WELCOMEWIDGET_H
#define WELCOMEWIDGET_H

#include <QWidget>

namespace Ui {
class WelcomeWidget;
}

class WelcomeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit WelcomeWidget(QWidget *parent = nullptr);
    ~WelcomeWidget();

private slots:
    void loadButton_clicked();
    void createButton_clicked();

Q_SIGNALS:
    void loadRequested();
    void createRequested();

private:
    Ui::WelcomeWidget *ui;
    void showOriginalContent();
};

#endif // WELCOMEWIDGET_H

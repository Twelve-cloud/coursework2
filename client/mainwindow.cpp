#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    connect(&authWindow, &AuthentificationWindow::registrationButtonClicked, [=](){ authWindow.hide(); regiWindow.show(); });
    connect(&regiWindow, &RegistrationWindow::backButtonClicked, [=]() { regiWindow.hide(); authWindow.show(); });
    authWindow.show();
}

MainWindow::~MainWindow()
{
    delete ui;
}


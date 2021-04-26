#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    connect(&authWindow, &AuthentificationWindow::registrationButtonClicked, [=](){ authWindow.hide(); regiWindow.show(); });
    connect(&regiWindow, &RegistrationWindow::backButtonClicked, [=]() { regiWindow.hide(); authWindow.show(); });
    connect(&regiWindow, &RegistrationWindow::registrationButtonClicked, this, &MainWindow::slotRegistrationClicked);
    connect(&authWindow, &AuthentificationWindow::signInButtonClicked, this, &MainWindow::slotSignInClicked);
    authWindow.show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotRegistrationClicked()
{

}

void MainWindow::slotSignInClicked()
{

}

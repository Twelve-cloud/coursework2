#include "authentification.h"
#include "ui_authentification.h"

AuthentificationWindow::AuthentificationWindow(QWidget *parent) : QWidget(parent), ui(new Ui::AuthentificationWindow)
{
    ui->setupUi(this);

    connect(ui -> registrationButton, &QPushButton::clicked, [=](){ emit registrationButtonClicked(); });
}

AuthentificationWindow::~AuthentificationWindow()
{
    delete ui;
}

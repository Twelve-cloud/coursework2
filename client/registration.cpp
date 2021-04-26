#include "registration.h"
#include "ui_registration.h"

RegistrationWindow::RegistrationWindow(QWidget *parent) : QWidget(parent), ui(new Ui::RegistrationWindow)
{
    ui->setupUi(this);

    connect(ui -> backButton, &QPushButton::clicked, [=](){ emit backButtonClicked(); });

}

RegistrationWindow::~RegistrationWindow()
{
    delete ui;
}

#include "authentification.h"
#include "ui_authentification.h"

AuthentificationWindow::AuthentificationWindow(QWidget* parent) : QWidget(parent), ui(new Ui::AuthentificationWindow)
{
    ui -> setupUi(this);

    connect(ui -> registrationButton, &QPushButton::clicked, [=]()
        {
            clearLines();
            emit registrationButtonClicked();
        });

    connect(ui -> authentificationButton, &QPushButton::clicked, this, &AuthentificationWindow::slotSignInClicked);
}


AuthentificationWindow::~AuthentificationWindow()
{
    delete ui;
}

void AuthentificationWindow::setError(const QString& error)
{
    ui -> errorLabel -> setText(error);
    ui -> errorLabel -> setStyleSheet("color: red");
}

bool AuthentificationWindow::isEmptyLines()
{
    if (login.isEmpty() || password.isEmpty())
        return true;

    return false;
}

void AuthentificationWindow::clearLines()
{
    ui -> loginLineEdit -> clear();
    ui -> passLineEdit -> clear();
    ui -> errorLabel -> setText("Аутентификация");
    ui -> errorLabel -> setStyleSheet("color: rgb(200, 200, 200)");
}

void AuthentificationWindow::slotSignInClicked()
{
    login = ui -> loginLineEdit -> text();
    password = ui -> passLineEdit -> text();
    clearLines();

    emit signInButtonClicked();
}

#include "authentification.h"
#include "ui_authentification.h"
#include <QKeyEvent>

AuthentificationWindow::AuthentificationWindow(QWidget* parent) : QWidget(parent), ui(new Ui::AuthentificationWindow)
{
    ui -> setupUi(this);

    connect(ui -> registrationButton, &QPushButton::clicked, [=]()
        {
            clearLines();
            emit registrationButtonClicked();
        });

    ui -> passLineEdit -> setEchoMode(QLineEdit::Password);
    ui -> authentificationButton -> setShortcut(Qt::Key_Return);

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

    emit signInButtonClicked();
}

void AuthentificationWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Down)
    {
        ui -> passLineEdit -> setFocus();
    }
    else if (event->key() == Qt::Key_Up)
    {
        ui -> loginLineEdit -> setFocus();
    }
}

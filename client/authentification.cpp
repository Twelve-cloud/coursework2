#include "authentification.h"
#include "ui_authentification.h"

AuthentificationWindow::AuthentificationWindow(QWidget *parent) : QWidget(parent), ui(new Ui::AuthentificationWindow)
{
    ui->setupUi(this);

    connect(ui -> registrationButton, &QPushButton::clicked, [=]()
        {
            clearLines();
            emit registrationButtonClicked();
        });

    connect(ui -> authentificationButton, &QPushButton::clicked, this, &AuthentificationWindow::slotSignInClicked);

}

bool AuthentificationWindow::isValidSize(const QString& strToValidation, const qint32 begin, const qint32 end)
{
    if (strToValidation.size() < begin || strToValidation.size() > end)
    {
        return false;
    }

    return true;
}

bool AuthentificationWindow::isValidContent(const QString& strToValidation)
{
    for (const auto& ch : strToValidation)
    {
        if (!ch.isDigit() && !ch.isLetter())
        {
            return false;
        }
    }

    return true;
}

void AuthentificationWindow::setError(const QString& error)
{
    ui -> errorLabel -> setText(error);
    ui -> errorLabel -> setStyleSheet("color: red");
}

void AuthentificationWindow::clearLines()
{
    ui -> loginLineEdit -> clear();
    ui -> passLineEdit -> clear();
    ui -> errorLabel -> setText("Аутентификация");
    ui -> errorLabel -> setStyleSheet("color: rgb(200, 200, 200)");
}

AuthentificationWindow::~AuthentificationWindow()
{
    delete ui;
}

void AuthentificationWindow::slotSignInClicked()
{
    if (!isValidContent(ui -> loginLineEdit -> text()))
    {
        setError("Логин должен содержить буквы или цифры");
    }
    else if (!isValidSize(ui -> loginLineEdit -> text(), 6, 16))
    {
        setError("Логин должен содержить от 6 до 16 символов");
    }
    else if (!isValidContent(ui -> passLineEdit -> text()))
    {
        setError("Пароль должен содержить буквы или цифры");
    }
    else if (!isValidSize(ui -> passLineEdit -> text(), 6, 32))
    {
        setError("Пароль должен содержить от 6 до 32 символов");
    }
    else
    {
        emit signInButtonClicked();
    }
}

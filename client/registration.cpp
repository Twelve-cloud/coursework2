#include "registration.h"
#include "ui_registration.h"

RegistrationWindow::RegistrationWindow(QWidget* parent) : QWidget(parent), ui(new Ui::RegistrationWindow)
{
    ui -> setupUi(this);

    connect(ui -> backButton, &QPushButton::clicked, [=]()
        {
            clearLines();
            emit backButtonClicked();
        });

    connect(ui -> registrationButton, &QPushButton::clicked, this, &RegistrationWindow::slotRegistrationClicked);
}

RegistrationWindow::~RegistrationWindow()
{
    delete ui;
}

bool RegistrationWindow::isValidSize(const QString& strToValidation, const qint32 begin, const qint32 end)
{
    if (strToValidation.size() < begin || strToValidation.size() > end)
        return false;

    return true;
}

bool RegistrationWindow::isValidContent(const QString& strToValidation)
{
    for (const auto& ch : strToValidation)
    {
        if (!ch.isDigit() && !ch.isLetter() && ch != '_')
        {
            return false;
        }
    }

    return true;
}

bool RegistrationWindow::isValidLoginFirstLetter(const QString& login)
{
    if (!login[0].isLetter() && login[0] != '_')
        return false;

    return true;
}

bool RegistrationWindow::isContainNecessarySymbols(const QString& strToValidation)
{
    std::size_t numbersCount = 0, lowLettersCount = 0, upperLettersCount = 0;

    for (const auto& ch : strToValidation)
    {
        if (ch.isLower())
        {
            ++lowLettersCount;
        }
        else if (ch.isUpper())
        {
            ++upperLettersCount;
        }
        else if (ch.isDigit())
        {
            ++numbersCount;
        }
    }

    if (numbersCount == 0 || lowLettersCount == 0 || upperLettersCount == 0)
    {
        return false;
    }

    return true;
}

void RegistrationWindow::setError(const QString& error)
{
    ui -> errorLabel -> setText(error);
    ui -> errorLabel -> setStyleSheet("color: red");
}

void RegistrationWindow::clearLines()
{
    ui -> loginLineEdit -> clear();
    ui -> passLineEdit -> clear();
    ui -> emailLineEdit -> clear();
    ui -> errorLabel -> setText("Регистрация");
    ui -> errorLabel -> setStyleSheet("color: rgb(200, 200, 200)");
}

void RegistrationWindow::slotRegistrationClicked()
{
    login = ui -> loginLineEdit -> text();
    password = ui -> passLineEdit -> text();
    email = ui -> emailLineEdit -> text();

    if (!isValidContent(login))
    {
        setError("Логин должен содержить буквы, цифры или символ \"_\"");
    }
    else if (!isValidSize(login, 6, 16))
    {
        setError("Логин должен содержить от 6 до 16 символов");
    }
    else if (!isValidLoginFirstLetter(login))
    {
        setError("Логин должен начинаться на букву или символ \"_\"");
    }
    else if (!isValidContent(password))
    {
        setError("Пароль должен содержить буквы, цифры или символ \"_\"");
    }
    else if (!isValidSize(password, 6, 32))
    {
        setError("Пароль должен содержить от 6 до 32 символов");
    }
    else if (!isContainNecessarySymbols(password))
    {
        setError("Пароль должен содержать цифру и символ в двух регистрах");
    }
    else
    {
        emit registrationButtonClicked();
    }
}

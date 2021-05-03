#ifndef REGISTRATION_H
#define REGISTRATION_H

#include <QWidget>

namespace Ui
{
    class RegistrationWindow;
}

class RegistrationWindow : public QWidget
{
    Q_OBJECT

public:
    explicit RegistrationWindow(QWidget *parent = nullptr);
    ~RegistrationWindow();

    bool isValidSize(const QString& strToValidation, const qint32 begin, const qint32 end);
    bool isValidContent(const QString& strToValidation);
    bool isValidLoginFirstLetter(const QString& login);
    bool isContainNecessarySymbols(const QString& strToValidation);
    void setError(const QString& error);
    void clearLines();

    QString getLogin() const noexcept { return login; }
    QString getPassword() const noexcept { return password; }

public slots:
    void slotRegistrationClicked();
    void keyPressEvent(QKeyEvent *event);

signals:
    void registrationButtonClicked();
    void backButtonClicked();

private:
    Ui::RegistrationWindow *ui;
    QString login, password;
};

#endif // REGISTRATION_H

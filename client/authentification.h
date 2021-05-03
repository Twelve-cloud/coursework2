#ifndef AUTHENTIFICATION_H
#define AUTHENTIFICATION_H

#include <QWidget>

namespace Ui
{
    class AuthentificationWindow;
}

class AuthentificationWindow : public QWidget
{
    Q_OBJECT

public:
    explicit AuthentificationWindow(QWidget *parent = nullptr);
    ~AuthentificationWindow();

    void setError(const QString& error);
    bool isEmptyLines();
    void clearLines();

    QString getLogin() const noexcept { return login; }
    QString getPassword() const noexcept { return password; }

public slots:
    void slotSignInClicked();
    void keyPressEvent(QKeyEvent *event);

signals:
    void signInButtonClicked();
    void registrationButtonClicked();

private:
    Ui::AuthentificationWindow *ui;
    QString login, password;
};

#endif // AUTHENTIFICATION_H

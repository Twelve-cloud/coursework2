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

    bool isValidSize(const QString& strToValidation, const qint32 begin, const qint32 end);
    bool isValidContent(const QString& strToValidation);
    void setError(const QString& error);
    void clearLines();

public slots:
    void slotSignInClicked();

signals:
    void signInButtonClicked();
    void registrationButtonClicked();

private:
    Ui::AuthentificationWindow *ui;
};

#endif // AUTHENTIFICATION_H

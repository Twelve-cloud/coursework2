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

signals:
    void registrationButtonClicked();

private:
    Ui::AuthentificationWindow *ui;
};

#endif // AUTHENTIFICATION_H

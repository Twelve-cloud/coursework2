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

signals:
    void backButtonClicked();

private:
    Ui::RegistrationWindow *ui;
};

#endif // REGISTRATION_H

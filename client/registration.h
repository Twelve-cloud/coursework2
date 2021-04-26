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
    void setError(const QString& error);
    void clearLines();

public slots:
    void slotRegistrationClicked();

signals:
    void registrationButtonClicked();
    void backButtonClicked();

private:
    Ui::RegistrationWindow *ui;
};

#endif // REGISTRATION_H

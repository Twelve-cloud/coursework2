#ifndef CONSULTANT_MAINWINDOW_H
#define CONSULTANT_MAINWINDOW_H

#include <QWidget>

namespace Ui {
class ConsultantMainWindow;
}

class ConsultantMainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ConsultantMainWindow(QWidget *parent = nullptr);
    ~ConsultantMainWindow();

    void addRequestLine(const QString& login);
    void deleteRequestLine(const QString& login);
    QString getLoginCurrentItem();

signals:
     void cancelRequestClicked();
     void acceptRequestClicked();

private:
    Ui::ConsultantMainWindow *ui;
};

#endif // CONSULTANT_MAINWINDOW_H

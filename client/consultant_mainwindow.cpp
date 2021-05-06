#include "consultant_mainwindow.h"
#include "ui_consultant_mainwindow.h"

ConsultantMainWindow::ConsultantMainWindow(QWidget *parent) : QWidget(parent), ui(new Ui::ConsultantMainWindow)
{
    ui -> setupUi(this);

    connect(ui -> cancelledRequestButton, &QPushButton::clicked, this, &ConsultantMainWindow::slotCancelRequest);
    connect(ui -> acceptRequestButton, &QPushButton::clicked, this, &ConsultantMainWindow::slotAcceptRequest);
}

ConsultantMainWindow::~ConsultantMainWindow()
{
    delete ui;
}

void ConsultantMainWindow::addRequestLine(const QString &login)
{
    ui ->  listOfRequests -> addItem(login);
}

void ConsultantMainWindow::deleteRequestLine(const QString &login)
{
    for (std::size_t i = 0; i < ui -> listOfRequests -> count(); i++)
    {
        if (ui -> listOfRequests -> item(i) -> text() == login)
        {
            ui -> listOfRequests -> takeItem(i);
            return;
        }
    }
}

QString ConsultantMainWindow::getLoginCurrentItem()
{
    return ui -> listOfRequests -> currentItem() -> text();
}

void ConsultantMainWindow::setError(const QString& error)
{
    ui -> mainLabel -> setText(error);
    ui -> mainLabel -> setStyleSheet("color: red");
}

void ConsultantMainWindow::slotCancelRequest()
{
    bool isFocused = false;
    for (std::size_t i = 0; i < ui -> listOfRequests -> count(); i++)
    {
        if (ui -> listOfRequests -> item(i) -> isSelected())
        {
            isFocused = true;
        }
    }
    if (isFocused)
    {
        emit cancelRequestClicked();
        ui -> mainLabel -> setText("Запросы на консультацию");
        ui -> mainLabel -> setStyleSheet("color: black");
    }
    else
    {
        setError("Ничего не выбрано");
    }
}

void ConsultantMainWindow::slotAcceptRequest()
{
    bool isFocused = false;
    for (std::size_t i = 0; i < ui -> listOfRequests -> count(); i++)
    {
        if (ui -> listOfRequests -> item(i) -> isSelected())
        {
            isFocused = true;
        }
    }
    if (isFocused)
    {
        emit acceptRequestClicked();
    }
    else
    {
        setError("Ничего не выбрано");
    }
}

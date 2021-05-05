#include "consultant_mainwindow.h"
#include "ui_consultant_mainwindow.h"

ConsultantMainWindow::ConsultantMainWindow(QWidget *parent) : QWidget(parent), ui(new Ui::ConsultantMainWindow)
{
    ui -> setupUi(this);

    connect(ui -> cancelledRequestButton, &QPushButton::clicked, this, [=](){ emit cancelRequestClicked(); });
    connect(ui -> acceptRequestButton, &QPushButton::clicked, this, [=](){ emit acceptRequestClicked(); });
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

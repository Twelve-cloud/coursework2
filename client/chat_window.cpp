#include "chat_window.h"
#include "ui_chat_window.h"

ChatWindow::ChatWindow(QWidget *parent) : QWidget(parent), ui(new Ui::ChatWindow)
{
    ui -> setupUi(this);

    connect(ui -> sendButton, &QPushButton::clicked, this, [=]()
                                                              {
                                                                  message = ui -> messageLineEdit -> text();
                                                                  ui -> messageLineEdit -> clear();
                                                                  emit sendMessageClicked();
                                                                });

    connect(ui -> endChatButton,  &QPushButton::clicked, this, [=]() { emit endChatClicked(); });
}

ChatWindow::~ChatWindow()
{
    delete ui;
}

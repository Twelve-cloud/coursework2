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

    connect(ui -> endChatButton,  &QPushButton::clicked, this, [=]() { ui -> chatView ->clear(); emit endChatClicked(); });
}

ChatWindow::~ChatWindow()
{
    delete ui;
}

void ChatWindow::clearChat()
{
    ui -> chatView -> clear();
}

void ChatWindow::renderMessage(const QString& role, const QString& message)
{
    QListWidgetItem* msg;
    if (role == "SENDER")
    {
        msg = new QListWidgetItem(message);
        msg -> setTextAlignment(Qt::AlignRight);
        msg -> setBackgroundColor(QColor(144, 238, 144));
    }
    else if (role == "RECIEVER")
    {
        msg = new QListWidgetItem(message);
        msg -> setTextAlignment(Qt::AlignLeft);
    }

    ui -> chatView -> addItem(msg);
}

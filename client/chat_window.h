#ifndef CHAT_WINDOW_H
#define CHAT_WINDOW_H

#include <QWidget>
#include <QList>

namespace Ui {
class ChatWindow;
}

class ChatWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ChatWindow(QWidget *parent = nullptr);
    ~ChatWindow();

    void setCompanion(const QString& companion_)
    {
        companion = companion_;
    }

    QString getCompanion() { return companion; }
    QString getMessage() { return message; }


    void clearChat();
    void renderMessage(const QString& role, const QString& message);

signals:
    void sendMessageClicked();
    void endChatClicked();

private:
    Ui::ChatWindow *ui;
    QString message;
    QString companion;
};

#endif // CHAT_WINDOW_H

#ifndef BOTCONTROLLER_H
#define BOTCONTROLLER_H

#include <QObject>
#include <telegrambotlib-qt-fork>

class BotController : public QObject
{
    Q_OBJECT
public:
    explicit BotController(QObject *parent = 0);


    TelegramBot *getBot() const;
    void setBot(TelegramBot *value);

    void setup();

    void start();

    void sendMessage(const QString &text, TelegramBotUpdate update);

    void editMessage(const QString &text, TelegramBotUpdate update, TelegramBotMessage *message);

    void sendMessageHtml();

    void sendPhotoWeb(const QString &address, const QString &text, TelegramBotUpdate update);

//    void sendAudioLocal()


private:
    TelegramBot *bot = nullptr;

signals:

public slots:
    void messageReceived(TelegramBotUpdate update);
    void testSendMessages(TelegramBotUpdate update);
    void testButtonClicked();
};

#endif // BOTCONTROLLER_H

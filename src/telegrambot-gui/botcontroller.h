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


    QString getChannelId() const;
    void setChannelId(const QString &newChannelId);
    void resetChannelId();

private:
    TelegramBot *bot = nullptr;
    QString channelId;

    Q_PROPERTY(QString channelId READ getChannelId WRITE setChannelId RESET resetChannelId NOTIFY channelIdChanged FINAL)

signals:

    void channelIdChanged();

public slots:
    void messageReceived(TelegramBotUpdate update);
    void testSendMessages(TelegramBotUpdate update);
    void testButtonClicked();
};

#endif // BOTCONTROLLER_H

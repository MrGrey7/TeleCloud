#include "botcontroller.h"

BotController::BotController(QObject *parent) : QObject(parent)
{

}

TelegramBot *BotController::getBot() const
{
    return bot;
}

void BotController::setBot(TelegramBot *value)
{
    bot = value;
}

void BotController::setup()
{
    QObject::connect(bot, SIGNAL(newMessage(TelegramBotUpdate)), this, SLOT(messageReceived(TelegramBotUpdate)));
//    connect(bot, &TelegramBot::newMessage, this, &BotController::messageReceived);
//    connect(bot, &TelegramBot::newMessage, [=](TelegramBotUpdate update) { qDebug() << "Slot invoked"; });
    start();
}

void BotController::start()
{
    bot->startMessagePulling();
}

void BotController::sendMessage(const QString &text, TelegramBotUpdate update)
{

}

void BotController::editMessage(const QString &text, TelegramBotUpdate update, TelegramBotMessage *message)
{

}

void BotController::sendMessageHtml()
{

}

void BotController::sendPhotoWeb(const QString &address, const QString &text, TelegramBotUpdate update)
{

}

QString BotController::getChannelId() const
{
    return channelId;
}

void BotController::setChannelId(const QString &newChannelId)
{
    if (channelId == newChannelId)
        return;
    channelId = newChannelId;
    emit channelIdChanged();
}

void BotController::resetChannelId()
{
    setChannelId({}); // TODO: Adapt to use your actual default value
}

void BotController::messageReceived(TelegramBotUpdate update)
{
    testSendMessages(update);
}

void BotController::testSendMessages(TelegramBotUpdate update)
{
    qDebug() << "BotController::testSendMessages()";
    // only handle Messages
    if(update->type != TelegramBotMessageType::Message) return;

    // simplify message access
    TelegramBotMessage& message = *update->message;

    // send message (Format: Normal)
    TelegramBotMessage msgSent;
    bot->sendMessage(message.chat.id,
                "This is a Testmessage",
                0,
                TelegramBot::NoFlag,
                TelegramKeyboardRequest(),
                &msgSent);

    // edit text of sent message (Format: Markdown)
    bot->editMessageText(message.chat.id,
                    msgSent.messageId,
                    "This is an edited *Testmessage*",
                    TelegramBot::Markdown);

    // send message (Format: HTML, Keyboard: Inline (2 Rows, 1 Column), Reply to Message: Yes)
    bot->sendMessage(message.chat.id,
                "Please <b>choose</b>",
                0,
                TelegramBot::Html,
                {
                    // Keyboard
                    {
                        TelegramBot::constructInlineButton("Google", "", "https://www.google.com"),
                    }, {
                        TelegramBot::constructInlineButton("Reply with data", "MYDATA1"),
                    }
                });

    // send photo (file location: web)
    bot->sendPhoto(message.chat.id,
              "https://www.kernel.org/theme/images/logos/tux.png",
              "This is the Linux Tux");

    // send audio (file location: local)
    bot->sendAudio(message.chat.id,
              "Maktone - Fluke01.mp3",
              "Listen to this great art :-)",
              "Maktone",
              "Fluke01");

    // send video chat note (file location: QByteArray)
    QFile file("testvideo.mp4");
    file.open(QFile::ReadOnly);
    QByteArray content = file.readAll();
    bot->sendVideoNote(msgSent.chat.id, content);

    // send sticker (file location: telegram server)
    bot->sendSticker(message.chat.id, "CXXXXXXXXXXXXXXXXXXXXXXX");
}

void BotController::testButtonClicked()
{
    // Send message to channel

    // send message (Format: Normal)
    TelegramBotMessage msgSent;
    bot->sendMessage(channelId,
                     "This is a Testmessage",
                     0,
                     TelegramBot::NoFlag,
                     TelegramKeyboardRequest(),
                     &msgSent);

    // send photo (file location: web)
    bot->sendPhoto(channelId,
                   "https://www.kernel.org/theme/images/logos/tux.png",
                   "This is the Linux Tux");

//    // send message (Format: HTML, Keyboard: Inline (2 Rows, 1 Column), Reply to Message: Yes)
//    bot->sendMessage(channelId,
//                     "Please <b>choose</b>",
//                     0,
//                     TelegramBot::Html,
//                     {
//                         // Keyboard
//                         {
//                             TelegramBot::constructInlineButton("Google", "", "https://www.google.com"),
//                         }, {
//                             TelegramBot::constructInlineButton("Reply with data", "MYDATA1"),
//                         }
//                     });

    // send audio (file location: local)
    bot->sendAudio(channelId,
                   "testfiles\\audio.mp3",
                   "Listen to this great art :-)",
                   "Author",
                   "Track");

//    // send video (file location: local)
//    bot->sendVideo(channelId,
//                   "testfiles\\video_8mb.mp4",
//                   "8MB Video");

    // send video (file location: local)
//    bot->sendVideo(channelId,
//                   "testfiles\\video_30mb.mp4",
//                   "30MB Video");

    // send video (file location: local)
    bot->sendVideo(channelId,
                   "testfiles\\video_100mb.mp4",
                   "100MB Video");

//    // send video (file location: local)
//    bot->sendVideo(channelId,
//                   "testfiles\\video_1gb.mp4",
//                   "1GB Video");

//    // send video (file location: local)
//    bot->sendVideo(channelId,
//                   "testfiles\\video_2gb.mp4",
//                   "2GB Video");

    qDebug() << "BotController::testButtonClicked()";
}

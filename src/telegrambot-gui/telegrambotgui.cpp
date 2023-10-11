#include "telegrambotgui.h"
#include <QSettings>

const QString botApiTokenKey = "BOT_API_TOKEN";
const QString channelIdKey  = "CHANNEL_ID";

TelegramBotGUI::TelegramBotGUI(QObject *parent) : QObject(parent)
{
    loadConfig();
    setup();
}

void TelegramBotGUI::loadConfig()
{
    QSettings settings("auth.ini",QSettings::IniFormat);
    botToken = settings.value(botApiTokenKey, "NOT FOUND").toString();
    channelId = settings.value(channelIdKey, "NOT FOUND").toString();
}

void TelegramBotGUI::setup()
{
    // MainWindow
    qDebug() << "Setting up mainWindow";
    MainWindow *mainWindow = new MainWindow();
    mainWindow->show();
    qDebug() << "finished";

    // BotController
    qDebug() << "Setting up botController";
    TelegramBot *bot = new TelegramBot(botToken);
    botController.setChannelId(channelId);
    botController.setBot(bot);
    botController.setup();
    qDebug() << "finished";

    // MessageBroker
    messageBroker.setMainWindow(mainWindow);
    messageBroker.setBotController(&botController);
}

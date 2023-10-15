#include "telegrambotgui.h"
#include <QSettings>

TelegramBotGUI::TelegramBotGUI(QObject *parent) : QObject(parent)
{
    loadConfig();
    setup();
}

void TelegramBotGUI::loadConfig()
{
    QSettings settingsAuth("auth.ini",QSettings::IniFormat);
    config.botToken = settingsAuth.value(botApiTokenKey, "NOT FOUND").toString();
    config.channelId = settingsAuth.value(channelIdKey, "NOT FOUND").toString();

    QSettings settingsConfig("config.ini",QSettings::IniFormat);
    config.recordingsJsonPath = settingsConfig.value(recordingsJsonPathKey, "NOT FOUND").toString();
    config.sqliteDbPath = settingsConfig.value(sqliteDbPathKey, "NOT FOUND").toString();
}

void TelegramBotGUI::setup()
{
    // Main Window
    qDebug() << "Setting up mainWindow";
    MainWindow *mainWindow = new MainWindow();
    mainWindow->show();
    qDebug() << "finished";

    // Bot Controller
    qDebug() << "Setting up botController";
    TelegramBot *bot = new TelegramBot(config.botToken);
    botController.setChannelId(config.channelId);
    botController.setBot(bot);
    botController.initialize();
    qDebug() << "finished";

    // Message Broker
    messageBroker.setMainWindow(mainWindow);
    messageBroker.setBotController(&botController);

    // Database Manager
    dbManager.setDbPath(config.sqliteDbPath);
    dbManager.setRecordingsJsonPath(config.recordingsJsonPath);
    dbManager.initialize();
}

#include "telegrambotgui.h"

// Needed to make_unique a TelegramBot
#include <telegrambotlib-qt-fork>

#include <QSettings>
#include <QDebug>
#include <QCoreApplication>

TelegramBotGUI::TelegramBotGUI(QObject *parent) : QObject(parent)
{
    loadConfig();
    setup();
}

void TelegramBotGUI::loadConfig()
{
    // Use QStringLiteral for file names
    QSettings settingsAuth(QStringLiteral("auth.ini"), QSettings::IniFormat);

    // Use constexpr keys from types.h (ConfigKeys::BotApiToken is const char*)
    // QSettings accepts const char* directly.
    m_config.botToken = settingsAuth.value(ConfigKeys::BotApiToken, QStringLiteral("NOT FOUND")).toString();
    m_config.channelId = settingsAuth.value(ConfigKeys::ChannelId, QStringLiteral("NOT FOUND")).toString();

    QSettings settingsConfig(QStringLiteral("config.ini"), QSettings::IniFormat);
    m_config.recordingsJsonPath = settingsConfig.value(ConfigKeys::RecordingsJsonPath, QStringLiteral("NOT FOUND")).toString();
    m_config.sqliteDbPath = settingsConfig.value(ConfigKeys::SqliteDbPath, QStringLiteral("NOT FOUND")).toString();
}

void TelegramBotGUI::setup()
{
    qDebug() << Q_FUNC_INFO;

    // Main Window
    m_mainWindow.show();

    // Database Manager
    m_dbManager.setDbPath(m_config.sqliteDbPath);
    m_dbManager.setRecordingsJsonPath(m_config.recordingsJsonPath);
    m_dbManager.initialize();

    // Bot Controller
    // Create unique_ptr using make_unique
    auto bot = std::make_unique<TelegramBot>(m_config.botToken);

    m_botController.setChannelId(m_config.channelId);
    // Move ownership of bot to controller
    m_botController.setBot(std::move(bot));
    m_botController.initialize();

    // Message Broker
    m_messageBroker.setMainWindow(&m_mainWindow);
    m_messageBroker.setBotController(&m_botController);
    m_messageBroker.setDbManager(&m_dbManager);
    m_messageBroker.connectSignals();
}

void TelegramBotGUI::showWindow()
{
    m_mainWindow.show();
}

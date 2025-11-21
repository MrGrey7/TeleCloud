#include "telegrambotgui.h"

// Needed to make_unique a TelegramBot
#include <telegrambotlib-qt-fork>

#include <QSettings>
#include <QDebug>
#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>

TelegramBotGUI::TelegramBotGUI(QObject *parent) : QObject(parent)
{
    loadConfig();
    setup();
}

void TelegramBotGUI::loadConfig()
{
    // Helper to resolve paths relative to the executable
    auto resolvePath = [](const QString &path) -> QString {
        if (path.isEmpty()) return path;
        QFileInfo fi(path);
        if (fi.isRelative()) {
            // If path is "telecloud.db", make it "C:/Path/To/App/bin/telecloud.db"
            return QDir(QCoreApplication::applicationDirPath()).filePath(path);
        }
        return path;
    };

    // Use QStringLiteral for file names
    // We look for INI files next to the executable
    QString appDir = QCoreApplication::applicationDirPath();
    QString authPath = QDir(appDir).filePath(QStringLiteral("auth.ini"));
    QString configPath = QDir(appDir).filePath(QStringLiteral("config.ini"));

    QSettings settingsAuth(authPath, QSettings::IniFormat);
    m_config.botToken = settingsAuth.value(ConfigKeys::BotApiToken, QStringLiteral("NOT FOUND")).toString();
    m_config.channelId = settingsAuth.value(ConfigKeys::ChannelId, QStringLiteral("NOT FOUND")).toString();

    QSettings settingsConfig(configPath, QSettings::IniFormat);

    // Load and resolve paths
    QString rawJsonPath = settingsConfig.value(ConfigKeys::RecordingsJsonPath, QStringLiteral("")).toString();
    m_config.recordingsJsonPath = resolvePath(rawJsonPath);

    QString rawDbPath = settingsConfig.value(ConfigKeys::SqliteDbPath, QStringLiteral("telecloud.db")).toString();
    m_config.sqliteDbPath = resolvePath(rawDbPath);

    qDebug() << "Configuration Loaded:";
    qDebug() << "  DB Path:" << m_config.sqliteDbPath;
    qDebug() << "  Json Path:" << m_config.recordingsJsonPath;
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

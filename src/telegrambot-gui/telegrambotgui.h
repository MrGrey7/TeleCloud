#pragma once

#include <QObject>
#include "types.h"
#include "mainwindow.h"
#include "messagebroker.h"
#include "botcontroller.h"
#include "dbmanager.h"

class TelegramBotGUI : public QObject
{
    Q_OBJECT
public:
    explicit TelegramBotGUI(QObject *parent = nullptr);

    void loadConfig();
    void setup();

    // FIX: Declare the missing method
    void showWindow();

private:
    // FIX: Define the members that were missing in the scope
    Config m_config;

    // Order implies destruction order.
    // MainWindow usually should be destroyed after Controller/Db to prevent signal issues during shutdown.
    DbManager     m_dbManager;
    BotController m_botController;
    MainWindow    m_mainWindow;
    MessageBroker m_messageBroker;
};

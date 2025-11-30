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

    void showWindow();

private:
    Config m_config;

    DbManager     m_dbManager;
    BotController m_botController;
    MainWindow    m_mainWindow;
    MessageBroker m_messageBroker;
};

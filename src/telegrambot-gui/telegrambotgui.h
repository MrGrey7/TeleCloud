#ifndef TELEGRAMBOTGUI_H
#define TELEGRAMBOTGUI_H

#include <QObject>
#include "mainwindow.h"
#include "messagebroker.h"
#include "botcontroller.h"
#include "dbmanager.h"
#include "types.h"

class TelegramBotGUI : public QObject
{
    Q_OBJECT
public:
    explicit TelegramBotGUI(QObject *parent = 0);
    void loadConfig();
    void setup();

private:
    MessageBroker messageBroker;
    BotController botController;
    DbManager dbManager;
    Config  config;

signals:

public slots:
};

#endif // TELEGRAMBOTGUI_H

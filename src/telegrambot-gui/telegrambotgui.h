#ifndef TELEGRAMBOTGUI_H
#define TELEGRAMBOTGUI_H

#include <QObject>
#include "mainwindow.h"
#include "messagebroker.h"

class TelegramBotGUI : public QObject
{
    Q_OBJECT
public:
    explicit TelegramBotGUI(QObject *parent = 0);
    void setup();

private:
    MessageBroker eventProcessor;
signals:

public slots:
};

#endif // TELEGRAMBOTGUI_H

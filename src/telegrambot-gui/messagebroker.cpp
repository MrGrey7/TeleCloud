#include "messagebroker.h"
//#include "iostream"
#include <QDebug>

MessageBroker::MessageBroker(QObject *parent) : QObject(parent)
{

}

MainWindow *MessageBroker::getMainWindow() const
{
    return mainWindow;
}

void MessageBroker::setMainWindow(MainWindow *value)
{
    if (mainWindow && mainWindow != value)
        mainWindow->disconnect(this);
    mainWindow = value;
    connectMainWindow();
}

void MessageBroker::connectMainWindow()
{
    connect(mainWindow, SIGNAL(testButtonClick()), this, SLOT(testButtonClicked()));
}

void MessageBroker::connectBotController()
{
    connect(this, SIGNAL(mainWindowTestButtonClick()), botController, SLOT(testButtonClicked()));
}

BotController *MessageBroker::getBotController() const
{
    return botController;
}

void MessageBroker::setBotController(BotController *value)
{
    botController = value;
}

void MessageBroker::testButtonClicked()
{
    qDebug() << "Test button click registered";
    emit mainWindowTestButtonClick();
}

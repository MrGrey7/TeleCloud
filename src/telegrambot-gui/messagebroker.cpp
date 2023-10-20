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
    connect(mainWindow, &MainWindow::sendGuiCommand, this, &MessageBroker::guiCommandReceived);
}

void MessageBroker::connectBotController()
{
    connect(this, &MessageBroker::mainWindowGuiCommandReceived, botController, &BotController::guiCommandReceived);
}

void MessageBroker::guiCommandReceived(GuiCommand command)
{
    qNamedDebug() << "received" << command.type;
    emit mainWindowGuiCommandReceived(command);
}

BotController *MessageBroker::getBotController() const
{
    return botController;
}

void MessageBroker::setBotController(BotController *value)
{
    botController = value;
    connectBotController();
}

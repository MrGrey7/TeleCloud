#include "messagebroker.h"
//#include "iostream"
#include <QDebug>


MessageBroker::MessageBroker(QObject *parent) : QObject(parent)
{

}

void MessageBroker::connectSignals()
{
    connectMainWindow();
    connectBotController();
    connectDbManager();
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
}

void MessageBroker::connectMainWindow()
{
    connect(mainWindow, &MainWindow::sendMessage, this, &MessageBroker::processMessage);
    connect(mainWindow, &MainWindow::sendMessage, botController, &BotController::processMessage);
    connect(mainWindow, &MainWindow::sendMessage, dbManager, &DbManager::processMessage);
}

void MessageBroker::connectBotController()
{
    connect(botController, &BotController::recordingUploaded, dbManager, &DbManager::writeUploadToDb);
    connect(botController, &BotController::recordingUploaded, mainWindow, &MainWindow::updateProcessedFileSize);
    connect(botController, &BotController::uploadQueueChanged, mainWindow, &MainWindow::updateQueueSize);
    connect(botController, &BotController::uploadsEnqueued, mainWindow, &MainWindow::updateTotalQueueFileSize);
}

void MessageBroker::connectDbManager()
{
    connect(dbManager, &DbManager::readMetadataProgressChanged, mainWindow, &MainWindow::updateMetadataProgress);
    connect(dbManager, &DbManager::loadedRecordingsToUpload, botController, &BotController::enqueueUploads);
}

DbManager *MessageBroker::getDbManager() const
{
    return dbManager;
}

void MessageBroker::setDbManager(DbManager *newDbManager)
{
    if (dbManager == newDbManager)
        return;
    dbManager = newDbManager;
}

void MessageBroker::processMessage(GenericMessage command)
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
}

#include "messagebroker.h"
#include "mainwindow.h"
#include "botcontroller.h"
#include "dbmanager.h"

#include <QDebug>

MessageBroker::MessageBroker(QObject *parent) : QObject(parent)
{
}

void MessageBroker::connectSignals()
{
    // We can only connect if all components are set
    if (!m_mainWindow || !m_botController || !m_dbManager) {
        qWarning() << "MessageBroker: Cannot connect signals, one or more components are missing.";
        return;
    }

    connectMainWindow();
    connectBotController();
    connectDbManager();
}

void MessageBroker::connectMainWindow()
{
    // GUI -> Broker
    connect(m_mainWindow, &MainWindow::sendMessage, this, &MessageBroker::processMessage);

    // GUI -> Bot
    connect(m_mainWindow, &MainWindow::sendMessage, m_botController, &BotController::processMessage);

    // GUI -> DB
    connect(m_mainWindow, &MainWindow::sendMessage, m_dbManager, &DbManager::processMessage);
}

void MessageBroker::connectBotController()
{
    // Bot -> DB
    connect(m_botController, &BotController::recordingUploaded, m_dbManager, &DbManager::writeUploadToDb);

    // Bot -> GUI
    connect(m_botController, &BotController::recordingUploaded, m_mainWindow, &MainWindow::updateProcessedFileSize);
    connect(m_botController, &BotController::uploadQueueChanged, m_mainWindow, &MainWindow::updateQueueSize);
    connect(m_botController, &BotController::uploadsEnqueued, m_mainWindow, &MainWindow::updateTotalQueueFileSize);
}

void MessageBroker::connectDbManager()
{
    // DB -> GUI
    connect(m_dbManager, &DbManager::readMetadataProgressChanged, m_mainWindow, &MainWindow::updateMetadataProgress);

    // DB -> Bot
    connect(m_dbManager, &DbManager::loadedRecordingsToUpload, m_botController, &BotController::enqueueUploads);
}

void MessageBroker::processMessage(const GenericMessage &command)
{
    qNamedDebug() << "received type:" << command.type;
    emit mainWindowGuiCommandReceived(command);
}

// Getters and Setters

MainWindow *MessageBroker::getMainWindow() const
{
    return m_mainWindow;
}

void MessageBroker::setMainWindow(MainWindow *value)
{
    if (m_mainWindow && m_mainWindow != value) {
        m_mainWindow->disconnect(this);
    }
    m_mainWindow = value;
}

BotController *MessageBroker::getBotController() const
{
    return m_botController;
}

void MessageBroker::setBotController(BotController *value)
{
    m_botController = value;
}

DbManager *MessageBroker::getDbManager() const
{
    return m_dbManager;
}

void MessageBroker::setDbManager(DbManager *newDbManager)
{
    m_dbManager = newDbManager;
}

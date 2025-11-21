#pragma once

#include <QObject>
#include "types.h"

// Forward declarations only. No need to include the full headers here.
class MainWindow;
class BotController;
class DbManager;

class MessageBroker : public QObject
{
    Q_OBJECT
public:
    explicit MessageBroker(QObject *parent = nullptr);

    void connectSignals();

    // Getters/Setters use raw pointers for observation (aggregation)
    MainWindow *getMainWindow() const;
    void setMainWindow(MainWindow *value);

    BotController *getBotController() const;
    void setBotController(BotController *value);

    DbManager *getDbManager() const;
    void setDbManager(DbManager *newDbManager);

public slots:
    void processMessage(const GenericMessage &command);

signals:
    void mainWindowGuiCommandReceived(GenericMessage);

private:
    void connectMainWindow();
    void connectBotController();
    void connectDbManager();

    // These are observing pointers (owned by TelegramBotGUI), so raw pointers are fine.
    MainWindow *m_mainWindow = nullptr;
    BotController *m_botController = nullptr;
    DbManager *m_dbManager = nullptr;
};

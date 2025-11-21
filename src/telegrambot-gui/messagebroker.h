#ifndef EVENTPROCESSOR_H
#define EVENTPROCESSOR_H

#include <QObject>
#include "mainwindow.h"
#include "botcontroller.h"
#include "dbmanager.h"
#include "types.h"

class MessageBroker : public QObject
{
    Q_OBJECT
public:
    explicit MessageBroker(QObject *parent = 0);

    void connectSignals();

    MainWindow *getMainWindow() const;
    void setMainWindow(MainWindow *value);

    BotController *getBotController() const;
    void setBotController(BotController *value);

    DbManager *getDbManager() const;
    void setDbManager(DbManager *newDbManager);

private:
    void connectMainWindow();
    void connectBotController();
    void connectDbManager();

private:
    MainWindow *mainWindow = nullptr;
    BotController *botController = nullptr;
    DbManager *dbManager = nullptr;
    
signals:
    void mainWindowGuiCommandReceived(GenericMessage);

public slots:
    void processMessage(GenericMessage command);
};

#endif // EVENTPROCESSOR_H

#ifndef EVENTPROCESSOR_H
#define EVENTPROCESSOR_H

#include <QObject>
#include "mainwindow.h"
#include "botcontroller.h"

class MessageBroker : public QObject
{
    Q_OBJECT
public:
    explicit MessageBroker(QObject *parent = 0);

    MainWindow *getMainWindow() const;
    void setMainWindow(MainWindow *value);

    BotController *getBotController() const;
    void setBotController(BotController *value);

private:
    void connectMainWindow();
    void connectBotController();

private:
    MainWindow *mainWindow = nullptr;
    BotController *botController = nullptr;

signals:
    void mainWindowTestButtonClick();

public slots:
    void testButtonClicked();
};

#endif // EVENTPROCESSOR_H

#include "telegrambotgui.h"


TelegramBotGUI::TelegramBotGUI(QObject *parent) : QObject(parent)
{
    setup();
}

void TelegramBotGUI::setup()
{
    // MainWindow
    qDebug() << "Setting up mainWindow";
    MainWindow *mainWindow = new MainWindow();
    messageBroker.setMainWindow(mainWindow);
    mainWindow->show();
    qDebug() << "finished";

    // BotController
    qDebug() << "Setting up botController";
    TelegramBot *bot = new TelegramBot("");
    botController.setBot(bot);
    botController.setup();
    qDebug() << "finished";
}

#include "telegrambotgui.h"


TelegramBotGUI::TelegramBotGUI(QObject *parent) : QObject(parent)
{
    setup();
}

void TelegramBotGUI::setup()
{
    MainWindow *mainWindow = new MainWindow();
    eventProcessor.setMainWindow(mainWindow);
    mainWindow->show();
}

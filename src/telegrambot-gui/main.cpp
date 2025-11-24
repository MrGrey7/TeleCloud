#include <QApplication>
#include <QLocale>
#include <QIcon>
#include "telegrambotgui.h"

int main(int argc, char *argv[])
{
    // QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);

    QLocale::setDefault(QLocale::C);

    app.setWindowIcon(QIcon(":/resources/appicon.png"));

    TelegramBotGUI telegramBotGUI;
    telegramBotGUI.showWindow();

    return app.exec();
}

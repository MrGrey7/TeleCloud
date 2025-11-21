#include <QApplication>
#include <QLocale>
#include <QIcon>
#include "telegrambotgui.h"

int main(int argc, char *argv[])
{
    // Modern Qt (6.x) handles HighDPI automatically, but good to be explicit in 5.x
    // QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);

    // Qt6 enforces UTF-8, no need for QTextCodec
    QLocale::setDefault(QLocale::C);

    app.setWindowIcon(QIcon(":/resources/appicon.png"));

    TelegramBotGUI telegramBotGUI;
    telegramBotGUI.showWindow(); // Explicitly showing window via the coordinator

    return app.exec();
}

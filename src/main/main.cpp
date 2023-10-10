
#include <QApplication>
#include <QDir>
#include <QFile>

#include <QTextCodec>
#include <QLocale>
#include <QDebug>

//#include <telegrambot-gui>

int main(int argc, char *argv[])
{
    qputenv( "LC_NUMERIC", "C" );
    QLocale::setDefault( QLocale::C );

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
    QTextCodec::setCodecForCStrings( QTextCodec::codecForName("utf-8") );
    QTextCodec::setCodecForTr( QTextCodec::codecForName("utf-8"));
#endif

    QApplication app(argc, argv);

    // Initializing TelegramBot-GUI
//    TelegramBotGUI telegramBotGUI;



    return app.exec();
}

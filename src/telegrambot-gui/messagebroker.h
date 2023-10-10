#ifndef EVENTPROCESSOR_H
#define EVENTPROCESSOR_H

#include <QObject>
#include "mainwindow.h"

class MessageBroker : public QObject
{
    Q_OBJECT
public:
    explicit MessageBroker(QObject *parent = 0);

    MainWindow *getMainWindow() const;
    void setMainWindow(MainWindow *value);

private:
    void connectMainWindow();

private:
    MainWindow *mainWindow = nullptr;

signals:

public slots:
    void testButtonClicked();
};

#endif // EVENTPROCESSOR_H

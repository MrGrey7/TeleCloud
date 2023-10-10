#ifndef BOTCONTROLLER_H
#define BOTCONTROLLER_H

#include <QObject>

class botcontroller : public QObject
{
    Q_OBJECT
public:
    explicit botcontroller(QObject *parent = 0);

signals:

public slots:
};

#endif // BOTCONTROLLER_H
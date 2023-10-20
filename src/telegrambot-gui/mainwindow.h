#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "types.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_button_download_clicked();

    void on_button_upload_clicked();

private:
    Ui::MainWindow *ui;

signals:
    void sendGuiCommand(GuiCommand);
};

#endif // MAINWINDOW_H

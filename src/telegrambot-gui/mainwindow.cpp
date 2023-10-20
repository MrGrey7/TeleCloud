#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QStandardPaths>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QString locations = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    qDebug() << locations;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_button_download_clicked()
{
    emit sendGuiCommand(GuiCommand(GuiCommand::Download));
}


void MainWindow::on_button_upload_clicked()
{
    emit sendGuiCommand(GuiCommand(GuiCommand::Upload));
}


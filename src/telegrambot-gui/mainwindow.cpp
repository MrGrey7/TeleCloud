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
    emit sendMessage(GenericMessage(GenericMessage::Download));
}


void MainWindow::on_button_upload_clicked()
{
    emit sendMessage(GenericMessage(GenericMessage::Upload));
}


void MainWindow::on_button_syncJsons_clicked()
{
    emit sendMessage(GenericMessage(GenericMessage::SyncMetadata));
}

void MainWindow::updateMetadataProgress(int value, int total)
{
    ui->progressBar_readMetadata->setMaximum(total);
    ui->progressBar_readMetadata->setValue(value);
}

void MainWindow::updateQueueSize(int size)
{
    ui->lbl_sizeValue->setText(QString("%1").arg(size));
}

void MainWindow::updateTotalQueueFileSize(qint64 queueSizeBytes)
{
    ui->progressBar_uploadVideos->setMaximum(queueSizeBytes / 1000000); // MB
}

void MainWindow::updateProcessedFileSize(RecordingUploadInfo upload)
{
    qint64 newTotalUploaded = ui->progressBar_uploadVideos->value();
    newTotalUploaded += upload.video.sizeBytes / 1000000;
    ui->progressBar_uploadVideos->setValue(newTotalUploaded);
}

void MainWindow::on_button_startUpload_clicked()
{
    emit sendMessage(GenericMessage(GenericMessage::UploadStart));
}


void MainWindow::on_button_stopUpload_clicked()
{
    emit sendMessage(GenericMessage(GenericMessage::UploadStop));
}


void MainWindow::on_button_fillQueue_clicked()
{
    emit sendMessage(GenericMessage(GenericMessage::FillQueue));
}


void MainWindow::on_button_updateFileStatus_clicked()
{
    emit sendMessage(GenericMessage(GenericMessage::UpdateFileStatus));
}


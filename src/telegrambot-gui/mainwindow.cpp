#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QStandardPaths>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(std::make_unique<Ui::MainWindow>()) // Modern smart pointer usage
{
    ui->setupUi(this);

    // Good for debugging
    qDebug() << "Data location:" << QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    ui->tabWidget->setTabEnabled(2, false);
}

MainWindow::~MainWindow() = default; // unique_ptr handles cleanup

// UI Slots
void MainWindow::on_button_download_clicked() {
    emit sendMessage(GenericMessage(GenericMessage::Download));
}

void MainWindow::on_button_upload_clicked() {
    emit sendMessage(GenericMessage(GenericMessage::Upload));
}

void MainWindow::on_button_syncJsons_clicked() {
    emit sendMessage(GenericMessage(GenericMessage::SyncMetadata));
}

void MainWindow::on_button_startUpload_clicked() {
    emit sendMessage(GenericMessage(GenericMessage::UploadStart));
}

void MainWindow::on_button_stopUpload_clicked() {
    emit sendMessage(GenericMessage(GenericMessage::UploadStop));
}

void MainWindow::on_button_fillQueue_clicked() {
    emit sendMessage(GenericMessage(GenericMessage::FillQueue));
}

void MainWindow::on_button_updateFileStatus_clicked() {
    emit sendMessage(GenericMessage(GenericMessage::UpdateFileStatus));
}

// Updates
void MainWindow::updateMetadataProgress(int value, int total) {
    ui->progressBar_readMetadata->setMaximum(total);
    ui->progressBar_readMetadata->setValue(value);
}

void MainWindow::updateQueueSize(int size) {
    ui->lbl_sizeValue->setText(QString::number(size));
}

void MainWindow::updateTotalQueueFileSize(qint64 queueSizeBytes) {
    // Use static_cast for intention
    ui->progressBar_uploadVideos->setMaximum(static_cast<int>(queueSizeBytes / 1000000));
}

void MainWindow::updateProcessedFileSize(const RecordingUploadInfo &upload) {
    int current = ui->progressBar_uploadVideos->value();
    int added = static_cast<int>(upload.video.sizeBytes / 1000000);
    ui->progressBar_uploadVideos->setValue(current + added);
}

#pragma once

#include <QMainWindow>
#include <memory> // for std::unique_ptr
#include "types.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

signals:
    void sendMessage(GenericMessage);

public slots:
    void updateMetadataProgress(int value, int total);
    void updateQueueSize(int size);
    void updateTotalQueueFileSize(qint64 queueSizeBytes);
    void updateProcessedFileSize(const RecordingUploadInfo &upload);

private slots:
    void on_button_download_clicked();
    void on_button_upload_clicked();
    void on_button_syncJsons_clicked();
    void on_button_startUpload_clicked();
    void on_button_stopUpload_clicked();
    void on_button_fillQueue_clicked();
    void on_button_updateFileStatus_clicked();

private:
    // Use unique_ptr for automatic memory management of the UI class
    std::unique_ptr<Ui::MainWindow> ui;

    qint64 m_queueSizeBytes = 0;
    qint64 m_processedSizeBytes = 0;
};

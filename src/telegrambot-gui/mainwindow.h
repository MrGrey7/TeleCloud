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

    void on_button_syncJsons_clicked();

    void on_button_startUpload_clicked();

    void on_button_stopUpload_clicked();

    void on_button_fillQueue_clicked();

    void on_button_updateFileStatus_clicked();

private:
    Ui::MainWindow *ui;
    qint64 queueSizeBytes = 0;
    qint64 processedSizeBytes = 0;

signals:
    void sendMessage(GenericMessage);

public slots:
    void updateMetadataProgress(int value, int total);
    void updateQueueSize(int size);
    void updateTotalQueueFileSize(qint64 queueSizeBytes);
    void updateProcessedFileSize(RecordingUploadInfo upload);

};

#endif // MAINWINDOW_H

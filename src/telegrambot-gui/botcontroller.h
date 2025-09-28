#ifndef BOTCONTROLLER_H
#define BOTCONTROLLER_H

#include <QObject>
#include <telegrambotlib-qt-fork>
#include <QQueue>
#include "types.h"

class BotController : public QObject
{
    Q_OBJECT
public:
    explicit BotController(QObject *parent = 0);


    TelegramBot *getBot() const;
    void setBot(TelegramBot *value);

    void initialize();

    void start();

    void sendMessage(const QString &text, TelegramBotUpdate update);

    void editMessage(const QString &text, TelegramBotUpdate update, TelegramBotMessage *message);

    void sendMessageHtml();

    void sendPhotoWeb(const QString &address, const QString &text, TelegramBotUpdate update);

//    void sendAudioLocal()

    TelegramBotMessage uploadVideo(const RecordingToUpload &upload);

    TelegramBotMessage uploadContactSheet(const RecordingToUpload &upload, int videoMessageId);

    QString getChannelId() const;
    void setChannelId(const QString &newChannelId);
    void resetChannelId();

    bool getPaused() const;
    void setPaused(bool newPaused);

private:
    TelegramBot *bot = nullptr;
    QString channelId;
    QQueue<RecordingToUpload> uploadQueue;
    bool paused = true;
    qint64 uploadQueueSizeBytes = 0;

    Q_PROPERTY(QString channelId READ getChannelId WRITE setChannelId RESET resetChannelId NOTIFY channelIdChanged FINAL)
    Q_PROPERTY(bool paused READ getPaused WRITE setPaused NOTIFY pausedChanged FINAL)

signals:
    void uploadEnqueued();
    void channelIdChanged();
//    void fileUploaded(QString path, int recordingId, QString fileId, int messageId, UploadTypes type);
    void recordingUploaded(RecordingUploadInfo info);
    void pausedChanged();
    void uploadQueueChanged(int size);
    void uploadsEnqueued(qint64 queueSizeBytes);

public slots:
    void testSendMessages(TelegramBotUpdate update);
    void testUpload();
    void testDownload();
    void processMessage(GenericMessage command);
    void enqueueVideo(RecordingToUpload upload);
    void enqueueUploads(const QVector<RecordingToUpload> &uploads);
    void startUploading();
};

#endif // BOTCONTROLLER_H

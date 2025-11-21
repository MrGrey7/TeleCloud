#pragma once

#include <QObject>
#include <QQueue>
#include <memory> // for std::unique_ptr
#include "types.h"

// FIX: Include the library here because we cannot forward declare
// TelegramBotUpdate if it is a typedef in the library.
#include <telegrambotlib-qt-fork>

class BotController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString channelId READ getChannelId WRITE setChannelId RESET resetChannelId NOTIFY channelIdChanged FINAL)
    Q_PROPERTY(bool paused READ getPaused WRITE setPaused NOTIFY pausedChanged FINAL)

public:
    explicit BotController(QObject *parent = nullptr);
    ~BotController() override;

    TelegramBot *getBot() const;
    void setBot(std::unique_ptr<TelegramBot> value);

    void initialize();
    void start();

    // FIX: Pass const reference to avoid copying shared pointers unnecessarily
    void sendMessage(const QString &text, const TelegramBotUpdate &update);
    void editMessage(const QString &text, const TelegramBotUpdate &update, TelegramBotMessage *message);
    void sendMessageHtml();
    void sendPhotoWeb(const QString &address, const QString &text, const TelegramBotUpdate &update);

    TelegramBotMessage uploadVideo(const RecordingToUpload &upload);
    TelegramBotMessage uploadContactSheet(const RecordingToUpload &upload, int videoMessageId);

    QString getChannelId() const;
    void setChannelId(const QString &newChannelId);
    void resetChannelId();

    bool getPaused() const;
    void setPaused(bool newPaused);

public slots:
    void testSendMessages(const TelegramBotUpdate &update);
    void testUpload();
    void testDownload();
    void processMessage(const GenericMessage &command);
    void enqueueVideo(const RecordingToUpload &upload);
    void enqueueUploads(const QVector<RecordingToUpload> &uploads);
    void startUploading();

signals:
    void uploadEnqueued();
    void channelIdChanged();
    void recordingUploaded(RecordingUploadInfo info);
    void pausedChanged();
    void uploadQueueChanged(int size);
    void uploadsEnqueued(qint64 queueSizeBytes);

private:
    std::unique_ptr<TelegramBot> m_bot;
    QString m_channelId;
    QQueue<RecordingToUpload> m_uploadQueue;
    bool m_paused = true;
    qint64 m_uploadQueueSizeBytes = 0;
};

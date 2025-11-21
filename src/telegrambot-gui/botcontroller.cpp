#include "botcontroller.h"

// We need the full definition of TelegramBot here for unique_ptr to generate the destructor
// Assuming the library header is available:
#include <telegrambotlib-qt-fork>

#include <QElapsedTimer>
#include <QUrl>
#include <QDebug>

BotController::BotController(QObject *parent) : QObject(parent)
{
    connect(this, &BotController::pausedChanged, this, &BotController::startUploading);
}

// Default destructor is now sufficient, unique_ptr handles cleanup.
// However, we must define it in the .cpp where TelegramBot is fully defined.
BotController::~BotController() = default;

TelegramBot *BotController::getBot() const
{
    return m_bot.get();
}

void BotController::setBot(std::unique_ptr<TelegramBot> value)
{
    // std::move transfers ownership to m_bot. Old m_bot is deleted automatically.
    m_bot = std::move(value);
}

void BotController::initialize()
{
    if(m_bot) {
        m_bot->startMessagePulling();
    }
}

void BotController::start()
{
    if(m_bot) m_bot->startMessagePulling();
}

// ... (Unchanged logic)

void BotController::testSendMessages(const TelegramBotUpdate &update)
{
    if (!m_bot || update->type != TelegramBotMessageType::Message) return;

    // Access via m_bot-> (unique_ptr overloads -> operator)
    TelegramBotMessage& message = *update->message;

    TelegramBotMessage msgSent;
    m_bot->sendMessage(message.chat.id,
                       QStringLiteral("This is a Testmessage"), // QStringLiteral usage
                       0,
                       TelegramBot::NoFlag,
                       TelegramKeyboardRequest(),
                       &msgSent);
    // ... rest of function
}

void BotController::testUpload()
{
    if (!m_bot) return;

    QElapsedTimer timer;
    timer.start();
    TelegramBotMessage response;

    // QStringLiteral for path is efficient if used multiple times
    m_bot->sendVideo(m_channelId,
                     QUrl::fromLocalFile(QStringLiteral("testfiles/video_2gb.mp4")),
                     QStringLiteral("2 gb video"),
                     -1, -1, -1, 0, TelegramBot::NoFlag, TelegramKeyboardRequest(),
                     &response);

    qint64 uploadTime = timer.elapsed();
    double seconds = (uploadTime > 0) ? (uploadTime / 1000.0) : 1.0;
    double speedMBSec = (response.video.fileSize / 1000000.0) / seconds;

    qDebug() << "Upload speed:" << speedMBSec << "MB/s";
}

void BotController::testDownload()
{
    if(m_bot) {
        // Example ID
        m_bot->getFile(QStringLiteral("BAACAgIAAx0EcBfiagADjmU1FWvhF7sCP0AgJpidU3VlN1dgAAK4OAACvvGxSTRWn94s_-2zLwQ"), true);
    }
}

void BotController::processMessage(const GenericMessage &command)
{
    qDebug() << "Bot processing:" << command.type;
    switch (command.type) {
    case GenericMessage::Upload:      testUpload(); break;
    case GenericMessage::Download:    testDownload(); break;
    case GenericMessage::UploadStart: setPaused(false); break;
    case GenericMessage::UploadStop:  setPaused(true); break;
    default: break;
    }
}

TelegramBotMessage BotController::uploadVideo(const RecordingToUpload &upload)
{
    TelegramBotMessage response;
    if (!m_bot) return response;

    QVariant videoSource;
    if (upload.videoFileId.isEmpty())
        videoSource = QUrl::fromLocalFile(upload.videoPath);
    else
        videoSource = upload.videoFileId;

    QElapsedTimer timer;
    timer.start();

    m_bot->sendVideo(m_channelId,
                     videoSource,
                     upload.caption,
                     -1, -1, -1, 0, TelegramBot::DisableNotification, TelegramKeyboardRequest(),
                     &response);

    qint64 uploadTime = timer.elapsed();
    double seconds = (uploadTime > 0) ? (uploadTime / 1000.0) : 1.0;
    double speedMBSec = (response.video.fileSize / 1000000.0) / seconds;

    qDebug().noquote() << QStringLiteral("Finished upload: %1, Size: %2 MB, Speed: %3 MB/s")
                              .arg(upload.videoPath)
                              .arg(response.video.fileSize / 1000000.0, 0, 'f', 2)
                              .arg(speedMBSec, 0, 'f', 2);

    return response;
}

TelegramBotMessage BotController::uploadContactSheet(const RecordingToUpload &upload, int videoMessageId)
{
    TelegramBotMessage response;
    if (!m_bot) return response;

    QVariant imageSource;
    if (upload.videoFileId.isEmpty())
        imageSource = QUrl::fromLocalFile(upload.contactSheetPath);
    else
        imageSource = upload.contactSheetFileId;

    m_bot->sendPhoto(m_channelId,
                     imageSource,
                     QString(),
                     videoMessageId, TelegramBot::DisableNotification, TelegramKeyboardRequest(),
                     &response);

    return response;
}

void BotController::enqueueVideo(const RecordingToUpload &upload)
{
    m_uploadQueue.append(upload);
    emit uploadQueueChanged(m_uploadQueue.size());
    emit uploadEnqueued();
}

void BotController::enqueueUploads(const QVector<RecordingToUpload> &uploads)
{
    for (const auto &upload : uploads) {
        m_uploadQueue.append(upload);
        m_uploadQueueSizeBytes += upload.sizeBytes;
    }
    emit uploadsEnqueued(m_uploadQueueSizeBytes);
    emit uploadQueueChanged(m_uploadQueue.size());
}

void BotController::startUploading()
{
    if (m_paused) return;

    while (!m_paused && !m_uploadQueue.isEmpty()) {
        RecordingToUpload item = m_uploadQueue.dequeue();

        qDebug().noquote() << QStringLiteral("Uploading: %1, Queue left: %2")
                                  .arg(item.videoPath)
                                  .arg(m_uploadQueue.size());

        TelegramBotMessage vidResp = uploadVideo(item);
        TelegramBotMessage imgResp = uploadContactSheet(item, vidResp.messageId);

        RecordingUploadInfo info;
        info.recordingId = item.recordingId;
        info.chatId = vidResp.chat.id;

        info.video.fileId = vidResp.video.fileId;
        info.video.messageId = vidResp.messageId;
        info.video.sizeBytes = vidResp.video.fileSize;

        if (!imgResp.photo.isEmpty()) {
            info.contactsheet.fileId = imgResp.photo.last().fileId;
        }
        info.contactsheet.messageId = imgResp.messageId;

        emit recordingUploaded(info);
        emit uploadQueueChanged(m_uploadQueue.size());
    }

    if (m_uploadQueue.isEmpty()) {
        qDebug() << "Queue empty, pausing.";
        setPaused(true);
    }
}

// Getters/Setters
QString BotController::getChannelId() const { return m_channelId; }
void BotController::setChannelId(const QString &val) {
    if (m_channelId == val) return;
    m_channelId = val;
    emit channelIdChanged();
}

bool BotController::getPaused() const { return m_paused; }
void BotController::setPaused(bool val) {
    if (m_paused == val) return;
    m_paused = val;
    emit pausedChanged();
}

void BotController::resetChannelId() {}

// Helper implementations
void BotController::sendMessage(const QString &text, const TelegramBotUpdate &update) {}
void BotController::editMessage(const QString &text, const TelegramBotUpdate &update, TelegramBotMessage *message) {}
void BotController::sendMessageHtml() {}
void BotController::sendPhotoWeb(const QString &address, const QString &text, const TelegramBotUpdate &update) {}

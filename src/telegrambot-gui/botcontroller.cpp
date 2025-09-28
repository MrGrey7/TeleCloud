#include "botcontroller.h"
#include <QElapsedTimer>
#include <QStringLiteral>

BotController::BotController(QObject *parent) : QObject(parent)
{
    connect(this, &BotController::pausedChanged, this, &BotController::startUploading);
}

TelegramBot *BotController::getBot() const
{
    return bot;
}

void BotController::setBot(TelegramBot *value)
{
    bot = value;
}

void BotController::initialize()
{
//    QObject::connect(bot, SIGNAL(newMessage(TelegramBotUpdate)), this, SLOT(processMessage(TelegramBotUpdate)));
//    connect(bot, &TelegramBot::newMessage, this, &BotController::processMessage);
//    connect(bot, &TelegramBot::newMessage, [=](TelegramBotUpdate update) { qDebug() << "Slot invoked"; });
    start();
}

void BotController::start()
{
    bot->startMessagePulling();
}

void BotController::sendMessage(const QString &text, TelegramBotUpdate update)
{

}

void BotController::editMessage(const QString &text, TelegramBotUpdate update, TelegramBotMessage *message)
{

}

void BotController::sendMessageHtml()
{

}

void BotController::sendPhotoWeb(const QString &address, const QString &text, TelegramBotUpdate update)
{

}

void BotController::enqueueVideo(RecordingToUpload upload)
{
    uploadQueue.append(upload);
    emit uploadQueueChanged(uploadQueue.size());
    emit uploadEnqueued();
}

QString BotController::getChannelId() const
{
    return channelId;
}

void BotController::setChannelId(const QString &newChannelId)
{
    if (channelId == newChannelId)
        return;
    channelId = newChannelId;
    emit channelIdChanged();
}

void BotController::resetChannelId()
{
    setChannelId({}); // TODO: Adapt to use your actual default value
}

bool BotController::getPaused() const
{
    return paused;
}

void BotController::setPaused(bool newPaused)
{
    if (paused == newPaused)
        return;
    qNamedDebug() << "paused = " << newPaused;
    paused = newPaused;
    emit pausedChanged();
}

void BotController::testSendMessages(TelegramBotUpdate update)
{
    qDebug() << "BotController::testSendMessages()";
    // only handle Messages
    if(update->type != TelegramBotMessageType::Message) return;

    // simplify message access
    TelegramBotMessage& message = *update->message;

    // send message (Format: Normal)
    TelegramBotMessage msgSent;
    bot->sendMessage(message.chat.id,
                "This is a Testmessage",
                0,
                TelegramBot::NoFlag,
                TelegramKeyboardRequest(),
                &msgSent);

    // edit text of sent message (Format: Markdown)
    bot->editMessageText(message.chat.id,
                    msgSent.messageId,
                    "This is an edited *Testmessage*",
                    TelegramBot::Markdown);

    // send message (Format: HTML, Keyboard: Inline (2 Rows, 1 Column), Reply to Message: Yes)
    bot->sendMessage(message.chat.id,
                "Please <b>choose</b>",
                0,
                TelegramBot::Html,
                {
                    // Keyboard
                    {
                        TelegramBot::constructInlineButton("Google", "", "https://www.google.com"),
                    }, {
                        TelegramBot::constructInlineButton("Reply with data", "MYDATA1"),
                    }
                });

    // send photo (file location: web)
    bot->sendPhoto(message.chat.id,
              "https://www.kernel.org/theme/images/logos/tux.png",
              "This is the Linux Tux");

    // send audio (file location: local)
    bot->sendAudio(message.chat.id,
              "Maktone - Fluke01.mp3",
              "Listen to this great art :-)",
              "Maktone",
              "Fluke01");

    // send video chat note (file location: QByteArray)
    QFile file("testvideo.mp4");
    file.open(QFile::ReadOnly);
    QByteArray content = file.readAll();
    bot->sendVideoNote(msgSent.chat.id, content);

    // send sticker (file location: telegram server)
    bot->sendSticker(message.chat.id, "CXXXXXXXXXXXXXXXXXXXXXXX");
}

void BotController::testUpload()
{
    // Send message to channel

    // send message (Format: Normal)
//    TelegramBotMessage msgSent;
//    bot->sendMessage(channelId,
//                     "This is a Testmessage",
//                     0,
//                     TelegramBot::NoFlag,
//                     TelegramKeyboardRequest(),
//                     &msgSent);

//    // send photo (file location: web)
//    bot->sendPhoto(channelId,
//                   "https://www.kernel.org/theme/images/logos/tux.png",
//                   "This is the Linux Tux");

//    // send message (Format: HTML, Keyboard: Inline (2 Rows, 1 Column), Reply to Message: Yes)
//    bot->sendMessage(channelId,
//                     "Please <b>choose</b>",
//                     0,
//                     TelegramBot::Html,
//                     {
//                         // Keyboard
//                         {
//                             TelegramBot::constructInlineButton("Google", "", "https://www.google.com"),
//                         }, {
//                             TelegramBot::constructInlineButton("Reply with data", "MYDATA1"),
//                         }
//                     });

    // send audio (file location: local)
    TelegramBotMessage response;

//    bot->sendAudio(channelId,
//                   "testfiles\\audio.mp3",
//                   "Listen to this great art :-)",
//                   "Author",
//                   "Track",
//                   -1, 0, TelegramBot::NoFlag, TelegramKeyboardRequest(),
//                    &response);

//    // send video (file location: local)
//    bot->sendVideo(channelId,
//                   "testfiles\\video_8mb.mp4",
//                   "8MB Video");

//     send video (file location: local)
//    bot->sendVideo(channelId,
//                   QUrl("testfiles//video_30mb.mp4"),
//                   "30MB Video",
//                   -1, -1, -1, 0, TelegramBot::NoFlag, TelegramKeyboardRequest(),
//                   &response);

    // send video (file location: local)
//    bot->sendVideo(channelId,
//                   "testfiles\\video_100mb.mp4",
//                   "100MB Video",
//                    -1, -1, -1, 0, TelegramBot::NoFlag, TelegramKeyboardRequest(),
//                    response);

    // send video (file location: local)
    QElapsedTimer timer;
    timer.start();
    bot->sendVideo(channelId,
                   QUrl::fromLocalFile("testfiles/video_2gb.mp4"),
                   "2 gb video",
                   -1, -1, -1, 0, TelegramBot::NoFlag, TelegramKeyboardRequest(),
                   &response);
    qint64 uploadTime = timer.elapsed();
    double speedMBSec = double(response.video.fileSize / ((double)uploadTime / 1000.0)) / 1000000.0;
    qNamedDebug() << "sending " << response.video.fileSize / 1000000 << "mb video took" << uploadTime << "ms, speed " << speedMBSec << "mb/sec";

    bot->sendPhoto(channelId,
                   QUrl::fromLocalFile("testfiles/ts_as_mp4.jpg"),
                   ".ts as .mp4 contactsheet",
                   response.messageId, TelegramBot::NoFlag, TelegramKeyboardRequest(),
                   &response);

//    // send video (file location: local)
//    bot->sendVideo(channelId,
//                   "testfiles\\video_2gb.mp4",
//                   "2GB Video");
    qNamedDebug() << "fileId original: " << response.video.fileId;

    qNamedDebug() << "uploading by fileId";
    QVariant variant(response.video.fileId);
    timer.restart();
    bot->sendVideo(channelId,
                   response.video.fileId,
                   "video upload by file id",
                   -1, -1, -1, 0, TelegramBot::NoFlag, TelegramKeyboardRequest(),
                   &response);
    qNamedDebug() << "sending 1GB video by fileId took" << timer.elapsed() << "ms";
    qNamedDebug() << "filedId: " << response.video.fileId;
}

void BotController::testDownload()
{
    TelegramBotFile file = bot->getFile("BAACAgIAAx0EcBfiagADjmU1FWvhF7sCP0AgJpidU3VlN1dgAAK4OAACvvGxSTRWn94s_-2zLwQ", true);
}

void BotController::processMessage(GenericMessage command)
{
    qNamedDebug() << command.type << sender();
    switch (command.type) {
        case GenericMessage::Upload:
            testUpload();
            break;
        case GenericMessage::Download:
            testDownload();
            break;
        case GenericMessage::UploadStart:
            setPaused(false);
            break;
        case GenericMessage::UploadStop:
            setPaused(true);
            break;
        default:
            break;
    }
}

TelegramBotMessage BotController::uploadVideo(const RecordingToUpload &upload)
{
    QVariant video;

    if (upload.videoFileId.isEmpty())
        video = QUrl::fromLocalFile(upload.videoPath);
    else
        video = upload.videoFileId;

    QElapsedTimer timer;
    timer.start();

    TelegramBotMessage response;
    bot->sendVideo(channelId,
                   video,
                   upload.caption,
                   -1, -1, -1, 0, TelegramBot::DisableNotification, TelegramKeyboardRequest(),
                   &response);

    qint64 uploadTime = timer.elapsed();
    double speedMBSec = double(response.video.fileSize / ((double)uploadTime / 1000.0)) / 1000000.0;
    qNamedDebug() << QString("Finished upload: %1 (RID %2), size = %3 MB, speed = %4 MB/s")
                         .arg(upload.videoPath).arg(upload.recordingId).arg(response.video.fileSize / 1000000).arg(speedMBSec);

    return response;
}

TelegramBotMessage BotController::uploadContactSheet(const RecordingToUpload &upload, int videoMessageId)
{
    QVariant image;

    if (upload.videoFileId.isEmpty())
        image = QUrl::fromLocalFile(upload.contactSheetPath);
    else
        image = upload.contactSheetFileId;

    QElapsedTimer timer;
    timer.start();

    TelegramBotMessage response;
    bot->sendPhoto(channelId,
                   image,
                   QString(),
                   videoMessageId, TelegramBot::DisableNotification, TelegramKeyboardRequest(),
                   &response);
    if (response.photo.isEmpty()) {
        qNamedDebug() << "error: response photo list empty";
        return response;
    }

    return response;
}

void BotController::enqueueUploads(const QVector<RecordingToUpload> &uploads)
{
    for (const RecordingToUpload &upload : uploads) {
        uploadQueue.append(upload);
        uploadQueueSizeBytes += upload.sizeBytes;
    }
    emit uploadsEnqueued(uploadQueueSizeBytes);
    emit uploadQueueChanged(uploadQueue.size());
}

void BotController::startUploading()
{
    if (paused)
        return;
    while (!paused && !uploadQueue.isEmpty()) {
        RecordingToUpload recordingToUpload = uploadQueue.dequeue();
        double recordingSizeGB = (double)recordingToUpload.sizeBytes / 1000000000.0;
        QString sizeString = QString::number(recordingSizeGB, 'f', 2);
        qNamedDebug() << QString("Starting upload: %1 (%2 GB), in queue = %3")
                            .arg(recordingToUpload.videoPath).arg(sizeString).arg(uploadQueue.size());

        TelegramBotMessage videoResponse = uploadVideo(recordingToUpload);
        TelegramBotMessage contactsheetResponse = uploadContactSheet(recordingToUpload, videoResponse.messageId);

        RecordingUploadInfo uploadInfo;
        uploadInfo.recordingId = recordingToUpload.recordingId;
        uploadInfo.chatId = videoResponse.chat.id;
        uploadInfo.video.fileId = videoResponse.video.fileId;
        uploadInfo.video.messageId = videoResponse.messageId;
        uploadInfo.video.sizeBytes = videoResponse.video.fileSize;
        if (!contactsheetResponse.photo.isEmpty())
            uploadInfo.contactsheet.fileId = contactsheetResponse.photo[0].fileId;
        uploadInfo.contactsheet.messageId = contactsheetResponse.messageId;


        emit recordingUploaded(uploadInfo);
        emit uploadQueueChanged(uploadQueue.size());
    }
    if (paused)
        qNamedDebug() << "Uploading was paused";
    else if (uploadQueue.isEmpty()) {
        qNamedDebug() << "Uploading stopped, queue empty";
        setPaused(true);
    }
}

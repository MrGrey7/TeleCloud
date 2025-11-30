#pragma once

#include <QString>
#include <QVariant>
#include <QMetaType>
#include <QDebug>

#define qNamedDebug() qDebug() << Q_FUNC_INFO


namespace ConfigKeys {
constexpr const char* BotApiToken        = "BOT_API_TOKEN";
constexpr const char* ChannelId          = "CHANNEL_ID";
constexpr const char* RecordingsJsonPath = "RECORDINGS_JSON_PATH";
constexpr const char* SqliteDbPath       = "SQLITE_DB_PATH";
}

struct Config {
    QString botToken;
    QString channelId;
    QString recordingsJsonPath;
    QString sqliteDbPath;
};

struct ChannelInfo {
    QString name;
    QString type;
    QString url;
};

struct RecordingToUpload {
    QString videoPath;
    QString contactSheetPath;
    QString caption;
    QString videoFileId;
    QString contactSheetFileId;
    qint64  sizeBytes = -1;
    int     recordingId = -1;
};
Q_DECLARE_METATYPE(RecordingToUpload)

struct UploadedFile {
    int messageId = -1;
    QString fileId;
    qint64 sizeBytes = -1;
};
Q_DECLARE_METATYPE(UploadedFile)

struct RecordingUploadInfo {
    int recordingId = -1;
    qint64 chatId = -1;
    UploadedFile video;
    UploadedFile contactsheet;
};
Q_DECLARE_METATYPE(RecordingUploadInfo)

struct RecordingMetadata {
    QString videoPath;
    QString contactSheetPath;
    QString metadataPath;
    QString generatedId;
    qint64  lastSizeUpdate = -1;
    ChannelInfo channelInfo;
    qint64  channelId = -1;
    bool    isPinned = false;
    int     progress = -1;
    int     selectedResolution = -1;
    bool    isSingleFile = false;
    qint64  sizeBytes = -1;
    qint64  startDate = -1;
    QString status;
};

struct GenericMessage {
    enum Type {
        // commands
        Upload,
        Download,
        SyncMetadata,
        FillQueue,
        UploadStart,
        UploadStop,
        UpdateFileStatus,

        // responses
        SyncedMetadata,
        FilledQueue,
        UploadStarted,
        UploadStopped
    };

    GenericMessage() = default;
    GenericMessage(Type messageType, const QString &messageParams = "")
        : type(messageType), params(messageParams) {}

    Type type = Upload;
    QVariant params;
};
Q_DECLARE_METATYPE(GenericMessage)

enum class UploadType {
    Recording
};

#ifndef TYPES_H
#define TYPES_H

#include <QString>
#include <QVariant>

#define qNamedDebug() qDebug() << Q_FUNC_INFO

// Config keys
const QString botApiTokenKey        = "BOT_API_TOKEN";
const QString channelIdKey          = "CHANNEL_ID";
const QString recordingsJsonPathKey = "RECORDINGS_JSON_PATH";
const QString sqliteDbPathKey       = "SQLITE_DB_PATH";

// Config fields
struct Config {
    QString botToken;
    QString channelId;
    QString recordingsJsonPath;
    QString sqliteDbPath;
};

// Model Data
struct ModelData {
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
    int     recordingId;
};

Q_DECLARE_METATYPE(RecordingToUpload)


struct UploadedFile {
    int messageId;
    QString fileId;
    qint64 sizeBytes;
};

Q_DECLARE_METATYPE(UploadedFile)

struct RecordingUploadInfo {
    int recordingId;
    int chatId;
    UploadedFile video;
    UploadedFile contactsheet;
};

Q_DECLARE_METATYPE(RecordingUploadInfo)

// Recording Metadata Fields to read from .json
struct RecordingMetadata {
    QString videoPath;
    QString contactSheetPath;
    QString metadataPath;
    QString generatedId;
    qint64  lastSizeUpdate = -1;
    ModelData modelData;
    qint64  modelId = -1;
    qint64  isPinned = -1;
    qint64  progress = -1;
    qint64  selectedResolution = -1;
    qint64  isSingleFile = -1;
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

    GenericMessage() {}
    GenericMessage(Type messageType, QString messageParams = "")
        : type(messageType), params(messageParams) {}
    Type type = Upload;
    QVariant params;
};

Q_DECLARE_METATYPE(GenericMessage)

enum UploadTypes {
    Recording
};

#endif // TYPES_H

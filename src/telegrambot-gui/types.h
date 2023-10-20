#ifndef TYPES_H
#define TYPES_H

#include <QString>

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

struct GuiCommand {
    enum Type {
        Upload,
        Download
    };

    GuiCommand() {}
    GuiCommand(Type commandType, QString commandParams = "")
        : type(commandType), params(commandParams) {}
    Type type = Upload;
    QString params;
};

Q_DECLARE_METATYPE(GuiCommand);

#endif // TYPES_H

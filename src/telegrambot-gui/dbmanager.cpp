#include "dbmanager.h"

#include <QFile>
#include <QDir>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>
#include <QLocale>
#include <QDebug>

namespace {
const QString ConnectionName = QStringLiteral("SQLite");
}

DbManager::DbManager(QObject *parent)
    : QObject{parent}
{
}

bool DbManager::openDb(const QString &path)
{
    if (!QFile::exists(path)) {
        qWarning() << "Db file missing. Creating at:" << path;
        if(!createDatabase(path)) return false;
    }

    QSqlDatabase db;
    if (QSqlDatabase::contains(ConnectionName)) {
        // Reuse existing connection
        db = QSqlDatabase::database(ConnectionName);
    } else {
        // Create new connection
        db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), ConnectionName);
    }
    db.setDatabaseName(path);

    if (!db.open()) {
        qCritical() << "Failed to open DB:" << path << db.lastError().text();
        return false;
    }
    return true;
}

bool DbManager::createDatabase(const QString &dbName)
{
    QSqlDatabase db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), ConnectionName);
    db.setDatabaseName(dbName);

    if (!db.open()) {
        qCritical() << "Unable to open database for creation.";
        return false;
    }

    QString fullQuery = R"(
        CREATE TABLE IF NOT EXISTS channels (channel_name TEXT PRIMARY KEY ON CONFLICT IGNORE, display_name TEXT, url TEXT, type TEXT);
        CREATE TABLE IF NOT EXISTS recordings (recording_id INTEGER PRIMARY KEY, video_path TEXT, video_exists INTEGER DEFAULT (- 1), contactsheet_path TEXT, contactsheet_exists INTEGER DEFAULT (- 1), metadata_path TEXT, generated_id TEXT UNIQUE ON CONFLICT IGNORE, last_size_update INTEGER, channel_name TEXT REFERENCES channels (channel_name), pinned TEXT, progress INTEGER, selected_resolution INTEGER, single_file TEXT, size_bytes INTEGER, start_date INTEGER, status TEXT, upload_id INTEGER REFERENCES uploads (upload_id) ON DELETE SET NULL);
        CREATE TABLE IF NOT EXISTS uploads (upload_id INTEGER PRIMARY KEY AUTOINCREMENT, chat_id INTEGER, video_message_id INTEGER, video_file_id TEXT, contactsheet_message_id INTEGER, contactsheet_file_id TEXT, date INTEGER, type TEXT);
        CREATE UNIQUE INDEX IF NOT EXISTS Recordings_upload_id_index ON recordings (upload_id COLLATE BINARY);
    )";

#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    const auto splitBehavior = Qt::SkipEmptyParts;
#else
    const auto splitBehavior = QString::SkipEmptyParts;
#endif
    QStringList queryList = fullQuery.split(';', splitBehavior);
    QSqlQuery q(db);

    for (const auto &queryText : queryList) {
        QString trimmed = queryText.trimmed();
        if (trimmed.isEmpty()) continue;

        if (!q.exec(trimmed)) {
            qWarning() << "Table creation failed:" << q.lastError().text();
            return false;
        }
    }
    return true;
}

void DbManager::readAllJsonMetadata()
{
    QDir directory(m_recordingsJsonPath);
    if (!directory.exists()) {
        qWarning() << "Metadata directory does not exist:" << m_recordingsJsonPath;
        return;
    }

    QStringList jsons = directory.entryList({QStringLiteral("*.json")}, QDir::Files);
    int total = jsons.size();

    QSet<QString> processed = getAllJsonsFromDb();

    int current = 0;
    for (const auto &filename : jsons) {
        current++;
        QString fullPath = directory.filePath(filename);

        if (processed.contains(fullPath)) continue;

        RecordingMetadata metadata;
        readMetadata(fullPath, metadata);

        if (metadata.status != QLatin1String("FINISHED")) continue;

        if (!writeRecordingToDb(metadata)) {
            qWarning() << "Failed to write recording:" << filename;
        }

        emit readMetadataProgressChanged(current, total);
    }
}

bool DbManager::writeRecordingToDb(const RecordingMetadata &metadata)
{
    QSqlDatabase db = QSqlDatabase::database(ConnectionName);
    QSqlQuery q(db);

    if (!metadata.channelInfo.name.isEmpty()) {
        q.prepare(QStringLiteral("INSERT OR IGNORE INTO channels (channel_name, url, type) VALUES (?, ?, ?)"));
        q.addBindValue(metadata.channelInfo.name);
        q.addBindValue(metadata.channelInfo.url);
        q.addBindValue(metadata.channelInfo.type);
        if (!q.exec()) qWarning() << "channel insert error:" << q.lastError().text();
    }

    q.prepare(QStringLiteral(
        "INSERT OR IGNORE INTO recordings "
        "(video_path, contactsheet_path, metadata_path, generated_id, last_size_update, "
        "channel_name, pinned, progress, selected_resolution, single_file, size_bytes, start_date, status) "
        "VALUES "
        "(:vp, :cp, :mp, :gid, :lsu, :cn, :pin, :prog, :res, :sf, :sb, :sd, :stat)"
        ));

    q.bindValue(QStringLiteral(":vp"), metadata.videoPath);
    q.bindValue(QStringLiteral(":cp"), metadata.contactSheetPath);
    q.bindValue(QStringLiteral(":mp"), metadata.metadataPath);
    q.bindValue(QStringLiteral(":gid"), metadata.generatedId);
    q.bindValue(QStringLiteral(":lsu"), metadata.lastSizeUpdate);
    q.bindValue(QStringLiteral(":cn"), metadata.channelInfo.name);
    q.bindValue(QStringLiteral(":pin"), metadata.isPinned);
    q.bindValue(QStringLiteral(":prog"), metadata.progress);
    q.bindValue(QStringLiteral(":res"), metadata.selectedResolution);
    q.bindValue(QStringLiteral(":sf"), metadata.isSingleFile);
    q.bindValue(QStringLiteral(":sb"), metadata.sizeBytes);
    q.bindValue(QStringLiteral(":sd"), metadata.startDate);
    q.bindValue(QStringLiteral(":stat"), metadata.status);

    if (!q.exec()) {
        qWarning() << "Recording insert error:" << q.lastError().text();
        return false;
    }
    return true;
}

void DbManager::readMetadata(const QString &path, RecordingMetadata &metadata)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonObject json = doc.object();

    metadata.metadataPath = path;
    metadata.videoPath = json[QStringLiteral("absoluteFile")].toString();
    metadata.generatedId = json[QStringLiteral("id")].toString();
    metadata.lastSizeUpdate = json[QStringLiteral("lastSizeUpdate")].toVariant().toLongLong();

    QJsonObject sourceObj;

    if (json.contains(QStringLiteral("channel"))) {
        sourceObj = json[QStringLiteral("channel")].toObject();
    }
    else if (json.contains(QStringLiteral("model"))) {
        sourceObj = json[QStringLiteral("model")].toObject();
    }

    metadata.channelInfo.name = sourceObj[QStringLiteral("name")].toString();
    metadata.channelInfo.url  = sourceObj[QStringLiteral("url")].toString();
    metadata.channelInfo.type = sourceObj[QStringLiteral("type")].toString();

    metadata.isPinned = json[QStringLiteral("pinned")].toBool();
    metadata.progress = json[QStringLiteral("progress")].toInt();
    metadata.sizeBytes = json[QStringLiteral("sizeInByte")].toVariant().toLongLong();
    metadata.startDate = json[QStringLiteral("startDate")].toVariant().toLongLong();
    metadata.status = json[QStringLiteral("status")].toString();

    if (json.contains(QStringLiteral("associatedFiles"))) {
        const QJsonArray files = json[QStringLiteral("associatedFiles")].toArray();
        for (const auto &val : files) {
            QString f = val.toString();
            if (f.endsWith(QStringLiteral(".jpg")) || f.endsWith(QStringLiteral(".png"))) {
                metadata.contactSheetPath = f;
                break;
            }
        }
    }
}

void DbManager::fillQueueWithRecordings()
{
    QSqlDatabase db = QSqlDatabase::database(ConnectionName);
    QSqlQuery q(db);

    q.prepare(QStringLiteral(
        "SELECT recording_id, video_path, contactsheet_path, channel_name, size_bytes, start_date "
        "FROM recordings "
        "WHERE upload_id IS NULL AND status = 'FINISHED' AND video_exists != 0 "
        "AND size_bytes > 20000000 AND size_bytes < 2000000000"
        ));

    if (!q.exec()) {
        qWarning() << "FillQueue query failed:" << q.lastError().text();
        return;
    }

    QVector<RecordingToUpload> selectedUploads;
    const int idxId = q.record().indexOf("recording_id");
    const int idxPath = q.record().indexOf("video_path");
    const int idxSheet = q.record().indexOf("contactsheet_path");
    const int idxchannel = q.record().indexOf("channel_name");
    const int idxSize = q.record().indexOf("size_bytes");
    const int idxDate = q.record().indexOf("start_date");

    while (q.next()) {
        RecordingToUpload upload;
        upload.recordingId = q.value(idxId).toInt();
        upload.videoPath = q.value(idxPath).toString();
        upload.contactSheetPath = q.value(idxSheet).toString();
        upload.sizeBytes = q.value(idxSize).toLongLong();
        upload.caption = generateCaption(q.value(idxchannel).toString(), q.value(idxDate).toLongLong());
        selectedUploads.append(upload);
    }

    emit loadedRecordingsToUpload(selectedUploads);
}

QString DbManager::generateCaption(const QString &channelName, qint64 date)
{
    QDateTime dt = QDateTime::fromMSecsSinceEpoch(date);
    QString caption = QStringLiteral("#") + channelName + ' ';

    if (dt.isValid()) {
        QLocale locale(QLocale::English);
        caption += QStringLiteral("#") + dt.toString(QStringLiteral("yyyy")) + "y ";
        caption += QStringLiteral("#") + locale.toString(dt, QStringLiteral("MMMM")) + ' ';
        caption += dt.toString(QStringLiteral("d, hh:mm:ss"));
    }
    return caption;
}

void DbManager::updateFileStatus()
{
    QSqlDatabase db = QSqlDatabase::database(ConnectionName);
    QSqlQuery q(db);
    q.exec(QStringLiteral("SELECT recording_id, video_path, video_exists, contactsheet_path, contactsheet_exists FROM recordings"));

    QSqlQuery updateQuery(db);

    db.transaction();

    while (q.next()) {
        QString vPath = q.value(1).toString();
        QString cPath = q.value(3).toString();

        bool vExists = QFile::exists(vPath);
        bool cExists = QFile::exists(cPath);

        if (q.value(2).toBool() != vExists || q.value(4).toBool() != cExists) {
            updateQuery.prepare(QStringLiteral("UPDATE recordings SET video_exists = ?, contactsheet_exists = ? WHERE recording_id = ?"));
            updateQuery.addBindValue(vExists ? 1 : 0);
            updateQuery.addBindValue(cExists ? 1 : 0);
            updateQuery.addBindValue(q.value(0).toInt());
            updateQuery.exec();
        }
    }
    db.commit();
}

QSet<QString> DbManager::getAllJsonsFromDb()
{
    QSet<QString> result;
    QSqlQuery q(QSqlDatabase::database(ConnectionName));
    if(q.exec(QStringLiteral("SELECT metadata_path FROM recordings"))) {
        while(q.next()) {
            result.insert(q.value(0).toString());
        }
    }
    return result;
}

void DbManager::processMessage(const GenericMessage &message)
{
    switch(message.type) {
    case GenericMessage::SyncMetadata: readAllJsonMetadata(); break;
    case GenericMessage::FillQueue: fillQueueWithRecordings(); break;
    case GenericMessage::UpdateFileStatus: updateFileStatus(); break;
    default: break;
    }
}

void DbManager::writeUploadToDb(const RecordingUploadInfo &upload)
{
    QSqlDatabase db = QSqlDatabase::database(ConnectionName);
    QSqlQuery q(db);

    q.prepare(QStringLiteral(
        "INSERT INTO uploads (chat_id, video_message_id, video_file_id, contactsheet_message_id, contactsheet_file_id, date, type) "
        "VALUES (:cid, :vmid, :vfid, :cmid, :cfid, :date, 'Recording')"
        ));

    q.bindValue(QStringLiteral(":cid"), upload.chatId);
    q.bindValue(QStringLiteral(":vmid"), upload.video.messageId);
    q.bindValue(QStringLiteral(":vfid"), upload.video.fileId);
    q.bindValue(QStringLiteral(":cmid"), upload.contactsheet.messageId);
    q.bindValue(QStringLiteral(":cfid"), upload.contactsheet.fileId);
    q.bindValue(QStringLiteral(":date"), QDateTime::currentMSecsSinceEpoch());

    if (!q.exec()) {
        qWarning() << "Upload insert failed:" << q.lastError().text();
        return;
    }

    QVariant uploadId = q.lastInsertId();

    QSqlQuery update(db);
    update.prepare(QStringLiteral("UPDATE recordings SET upload_id = :uid WHERE recording_id = :rid"));
    update.bindValue(QStringLiteral(":uid"), uploadId);
    update.bindValue(QStringLiteral(":rid"), upload.recordingId);

    if (!update.exec()) {
        qWarning() << "Link recording to upload failed:" << update.lastError().text();
    }
}

QString DbManager::getDbPath() const { return m_dbPath; }
void DbManager::setDbPath(const QString &path) {
    if (m_dbPath == path) return;
    m_dbPath = path;
    emit dbPathChanged();
}

QString DbManager::getRecordingsJsonPath() const { return m_recordingsJsonPath; }
void DbManager::setRecordingsJsonPath(const QString &path) {
    if (m_recordingsJsonPath == path) return;
    m_recordingsJsonPath = path;
    emit recordingsJsonPathChanged();
}

void DbManager::initialize() {
    openDb(m_dbPath);
}

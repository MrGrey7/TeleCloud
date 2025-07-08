#include "dbmanager.h"

#include <QFile>
#include <QDir>

#include <QSqlQuery>
#include <QSqlDriver>
#include <QSqlRecord>
#include <QSqlField>
#include <QSqlError>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QDateTime>
#include <QLocale>

#include <QDebug>


const QString sqlConnectionName = "SQLite";
//const QString sqlUsername = "TelegramBot";

DbManager::DbManager(QObject *parent)
    : QObject{parent}
{

}

bool DbManager::openDb(const QString &path)
{
    if (!QFile::exists(dbPath)) {
        // DB file does not exist
        qDebug() << "DbManager::openDb: error: DB file does not exist at " << path;
    }
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", sqlConnectionName);
    db.setDatabaseName(dbPath);
//    db.setUserName(sqlUsername);
    if (!db.open()) {
        qDebug() << "DbManager::DbManager(): error: failed to open " << path;
        return false;
    }
    return true;
//    QStringList tables = db.tables();
////    QVector<QSqlRecord *> records;
//    for (auto table : tables) {
//        QSqlRecord record = db.record(table);;
//        qDebug() << "Table " << table << ", " << record.count() << " fields";
//        for (int i = 0; i < record.count(); i++) {
//            QSqlField field = record.field(i);
//            qDebug() << "Name: " << field.name() << "; type: " << field.type();
//        }

//    }

}

void DbManager::readJsonTest()
{
    QFile file(R"()");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString text = file.readAll();
    file.close();
//    qDebug() << text;
    QJsonDocument doc = QJsonDocument::fromJson(text.toUtf8());
    QJsonObject object = doc.object();
    auto models = object["model"].toObject();
    QStringList keys = object.keys();
}

void DbManager::readAllJsonMetadata()
{
    QDir directory(recordingsJsonPath);
    if (!directory.exists()) {
        qWarning() << "DbManager::readAllJsonMetadata: error: metadata directory" << recordingsJsonPath << "does not exist";
        return;
    }
    if (!QSqlDatabase::database(sqlConnectionName).isOpen()) {
        qWarning() << "DbManager::readAllJsonMetadata: error: database is not opened";
        return;
    }

    QStringList jsons = directory.entryList(QStringList() << "*.json", QDir::Files);
    int n = jsons.size();
    QSet<QString> alreadyProcessedJsons = getAllJsonsFromDb();
    for(int i = 0; i < n; i++) {
        QString &filename = jsons[i];
        if (alreadyProcessedJsons.contains(filename))
            continue;
        RecordingMetadata metadata;
        readMetadata(QString(R"RX(%1/%2)RX").arg(recordingsJsonPath).arg(filename),
                     metadata);
        if (metadata.status.compare("FINISHED") != 0) // if recording is not FINISHED skip it
            continue;
        if (!writeRecordingToDb(metadata)) {
            qWarning() << "DbManager::readAllJsonMetadata: error: writeRecordingToDb() failed for " << filename;
        }
        emit readMetadataProgressChanged(i + 1, n);
    }
}

bool DbManager::writeRecordingToDb(RecordingMetadata &metadata)
{
    QSqlDatabase db = QSqlDatabase::database(sqlConnectionName);

    // Inserting the model into "models" table if they don't exist yet
    if (!metadata.modelData.name.isEmpty()) {
        QSqlQuery q(db);
        q.prepare(QString("INSERT OR IGNORE INTO models (model_name, url, type) "
                          "VALUES (?, ?, ?)"));
        q.addBindValue(metadata.modelData.name);
        q.addBindValue(metadata.modelData.url);
        q.addBindValue(metadata.modelData.type);

        if (!q.exec()) {
            qWarning() << "Query failed: " << q.executedQuery();
            qWarning() << "DB text: " << q.lastError().databaseText();
            qWarning() << "Driver text: " << q.lastError().driverText();
            return false;
        }
    } else
        qWarning("DbManager::syncJsonMetadata: error: empty model name");

    // Inserting the recording metadata into the "recordings" table
    if (!metadata.modelData.name.isEmpty()) {
        QSqlQuery q(db);
        q.prepare(QString("INSERT OR IGNORE INTO recordings (video_path, contactsheet_path, metadata_path, generated_id, last_size_update, model_name,"
                          "pinned, progress, selected_resolution, single_file, size_bytes, start_date, status) "
                          "VALUES (:video_path, :contactsheet_path, :metadata_path, :generated_id, :last_size_update, :model_name,"
                          ":pinned, :progress, :selected_resolution, :single_file, :size_bytes, :start_date, :status)"));
        q.bindValue(":video_path", metadata.videoPath);
        q.bindValue(":contactsheet_path", metadata.contactSheetPath);
        q.bindValue(":metadata_path", metadata.metadataPath);
        q.bindValue(":generated_id", metadata.generatedId);
        q.bindValue(":last_size_update", metadata.lastSizeUpdate);
        q.bindValue(":model_name", metadata.modelData.name);
        q.bindValue(":pinned", metadata.isPinned);
        q.bindValue(":progress", metadata.progress);
        q.bindValue(":selected_resolution", metadata.selectedResolution);
        q.bindValue(":single_file", metadata.isSingleFile);
        q.bindValue(":size_bytes", metadata.sizeBytes);
        q.bindValue(":start_date", metadata.startDate);
        q.bindValue(":status", metadata.status);
        if (!q.exec()) {
            qWarning("DbManager::syncJsonMetadata: error: recording insertion failed, error message:");
            qWarning() << "Query failed: " << q.executedQuery();
            qWarning() << "DB text: " << q.lastError().databaseText();
            qWarning() << "Driver text: " << q.lastError().driverText();
            return false;
        }
    } else {
        qWarning("DbManager::syncJsonMetadata: error: recording with no video file path");
        return false;
    }
    return true;
}

void DbManager::readMetadata(const QString &metadataJsonPath, RecordingMetadata &metadata)
{
    QFile file(metadataJsonPath);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString text = file.readAll();
    file.close();
//    qDebug() << text;
    QJsonDocument doc = QJsonDocument::fromJson(text.toUtf8());
    QJsonObject json = doc.object();

    if (const QJsonValue v = json["absoluteFile"]; v.isString())
        metadata.videoPath = v.toString();

    if (const QJsonValue v = json["associatedFiles"]; v.isArray()) {
        const QJsonArray files = v.toArray();
        for (const QJsonValue &filePath : files) {
            if (filePath.toString().endsWith(".jpg") || filePath.toString().endsWith(".png")) {
                metadata.contactSheetPath = filePath.toString();
                break;
            }
        }
    }

    if (const QJsonValue v = json["id"]; v.isString())
        metadata.generatedId = v.toString();

    if (const QJsonValue v = json["lastSizeUpdate"]; v.isDouble())
        metadata.lastSizeUpdate = v.toVariant().toLongLong();

    if (const QJsonValue v = json["metaDataFile"]; v.isString())
        metadata.metadataPath = v.toString();

    if (const QJsonValue v = json["model"]; v.isObject()) {
        QJsonObject model = v.toObject();

        if (const QJsonValue mv = model["name"]; mv.isString())
            metadata.modelData.name = mv.toString();

        if (const QJsonValue mv = model["url"]; mv.isString())
            metadata.modelData.url = mv.toString();

        if (const QJsonValue mv = model["type"]; mv.isString())
            metadata.modelData.type = mv.toString();
    }

    if (const QJsonValue v = json["pinned"]; v.isBool())
        metadata.isPinned = v.toBool();

    if (const QJsonValue v = json["progress"]; v.isDouble())
        metadata.progress = v.toVariant().toLongLong();

    if (const QJsonValue v = json["selectedResolution"]; v.isDouble())
        metadata.selectedResolution = v.toVariant().toLongLong();

    if (const QJsonValue v = json["singleFile"]; v.isBool())
        metadata.isSingleFile = v.toBool();

    if (const QJsonValue v = json["sizeInByte"]; v.isDouble())
        metadata.sizeBytes = v.toVariant().toLongLong();

    if (const QJsonValue v = json["startDate"]; v.isDouble())
        metadata.startDate = v.toVariant().toLongLong();

    if (const QJsonValue v = json["status"]; v.isString())
        metadata.status = v.toVariant().toString();
}

void DbManager::fillQueueWithRecordings()
{
    QSqlDatabase db = QSqlDatabase::database(sqlConnectionName);
    if (!db.isOpen()) {
        qNamedDebug() << "error: db is not opened";
        return;
    }
    QSqlQuery q(db);
    q.prepare(R"(select recording_id, video_path, video_exists, contactsheet_path, contactsheet_exists,
                model_name, size_bytes, status, start_date, upload_id
                from recordings
                where upload_id IS NULL and status = 'FINISHED' and video_exists != 0
                and size_bytes > 20000000 and size_bytes < 2000000000;)");

    if (!q.exec()) {
        qWarning() << "Query failed: " << q.executedQuery();
        qWarning() << "DB text: " << q.lastError().databaseText();
        qWarning() << "Driver text: " << q.lastError().driverText();
    }
    int videoPathColumn = q.record().indexOf("video_path");
    int recordingIdColumn = q.record().indexOf("recording_id");
    int modelNameColumn = q.record().indexOf("model_name");
    int contactSheetColumn = q.record().indexOf("contactsheet_path");
    int startDateColumn = q.record().indexOf("start_date");
    int sizeBytsColumn = q.record().indexOf("size_bytes");
//    int n = 0;
    QVector<RecordingToUpload> selectedUploads;
    while (q.next()) {
//        n++;
        RecordingToUpload upload;
        upload.videoPath = q.value(videoPathColumn).toString();
        upload.contactSheetPath = q.value(contactSheetColumn).toString();
        upload.recordingId = q.value(recordingIdColumn).toInt();
        upload.caption = generateCaption(q.value(modelNameColumn).toString(), q.value(startDateColumn).toLongLong());
        upload.sizeBytes = q.value(sizeBytsColumn).toLongLong();
        selectedUploads.append(upload);
    }

    emit loadedRecordingsToUpload(selectedUploads);
}

QString DbManager::generateCaption(QString modelName, qint64 date)
{
    QDateTime dt = QDateTime::fromMSecsSinceEpoch(date);
    QString caption;
    caption += '#' + modelName + ' ';
    if (dt.isValid()) {
        QLocale locale(QLocale("en_US"));
        caption += '#' + dt.date().toString("yyyy") + "y ";
        caption += '#' + locale.toString(dt, "MMMM") + ' ';
        caption += dt.date().toString("d") + ", ";
        caption += dt.time().toString();
    }
    return caption;
}

void DbManager::updateFileStatus()
{
    QSqlDatabase db = QSqlDatabase::database(sqlConnectionName);
    if (!db.isOpen()) {
        qNamedDebug() << "error: db is not opened";
        return;
    }
    QSqlQuery q(db);
    q.prepare("select recording_id, video_path, video_exists, contactsheet_path, contactsheet_exists "
              "from recordings;");

    if (!q.exec()) {
        qWarning() << "Query failed: " << q.executedQuery();
        qWarning() << "DB text: " << q.lastError().databaseText();
        qWarning() << "Driver text: " << q.lastError().driverText();
    }
    int recordingIdColumn = q.record().indexOf("recording_id");
    int videoPathColumn = q.record().indexOf("video_path");
    int videoExistsColumn = q.record().indexOf("video_exists");
    int contactSheetPathColumn = q.record().indexOf("contactsheet_path");
    int contactSheetExistsColumn = q.record().indexOf("contactsheet_exists");

    while (q.next()) {
        int oldVideoExists = q.value(videoExistsColumn).toInt();
        int oldContactSheetExists = q.value(contactSheetExistsColumn).toInt();
        int videoExists = QFile::exists(q.value(videoPathColumn).toString());
        int contactSheetExists = QFile::exists(q.value(contactSheetPathColumn).toString());
        if (oldVideoExists != videoExists || oldContactSheetExists != contactSheetExists) {
            QSqlQuery update(db);
            update.prepare(QString("UPDATE recordings SET video_exists = %1, contactsheet_exists = %2 WHERE recording_id = %3")
                               .arg(videoExists).arg(contactSheetExists).arg(q.value(recordingIdColumn).toInt()));

            if (!update.exec()) {
                qWarning() << "Query failed: " << update.executedQuery();
                qWarning() << "DB text: " << update.lastError().databaseText();
                qWarning() << "Driver text: " << update.lastError().driverText();
            }
        }

    }
}

QSet<QString> DbManager::getAllJsonsFromDb()
{
    QSqlQuery q(QSqlDatabase::database(sqlConnectionName));
    q.prepare("select metadata_path "
              "from recordings;");

    if (!q.exec()) {
        qNamedDebug() << "Query failed: " << q.executedQuery();
        qWarning() << "DB text: " << q.lastError().databaseText();
        qWarning() << "Driver text: " << q.lastError().driverText();
    }
    QSet<QString> result;
    while (q.next()) {
        result.insert(q.value(0).toString());
    }
    return result;
}

void DbManager::processMessage(GenericMessage message)
{
    if (message.type == GenericMessage::SyncMetadata)
        readAllJsonMetadata();
    if (message.type == GenericMessage::FillQueue)
        fillQueueWithRecordings();
    if (message.type == GenericMessage::UpdateFileStatus)
        updateFileStatus();
}

void DbManager::writeUploadToDb(RecordingUploadInfo upload)
{
    QSqlDatabase db = QSqlDatabase::database(sqlConnectionName);
    if (!db.isOpen()) {
        qNamedDebug() << "error: db is not opened";
        return;
    }
    QSqlQuery q(db);
    // Insert into video_uploads table
    q.prepare(QString("INSERT OR IGNORE INTO uploads (chat_id, video_message_id, video_file_id, "
                      "contactsheet_message_id, contactsheet_file_id, date, type)"
                      "VALUES (:chat_id, :video_message_id, :video_file_id,"
                      ":contactsheet_message_id, :contactsheet_file_id, :date, :type)"));
    q.bindValue(":chat_id", upload.chatId);
    q.bindValue(":video_message_id", upload.video.messageId);
    q.bindValue(":video_file_id", upload.video.fileId);
    q.bindValue(":contactsheet_message_id", upload.contactsheet.messageId);
    q.bindValue(":contactsheet_file_id", upload.contactsheet.fileId);
    q.bindValue(":date", QDateTime::currentMSecsSinceEpoch());
    q.bindValue(":type", "Recording");
    if (!q.exec()) {
        qNamedDebug() << "Query failed: " << q.executedQuery();
        qWarning() << "DB text: " << q.lastError().databaseText();
        qWarning() << "Driver text: " << q.lastError().driverText();
        return;
    } else {
        qNamedDebug() << QString("Successfully inserted upload, id = %2")
                             .arg(q.lastInsertId().toInt());
    }
    int insertedId = q.lastInsertId().toInt();

    // update recordings table
    q.prepare(QString("UPDATE recordings SET upload_id = %1 WHERE recording_id = %2")
              .arg(insertedId).arg(upload.recordingId));
    if (!q.exec()) {
        qNamedDebug() << "Query failed: " << q.executedQuery();
        qWarning() << "DB text: " << q.lastError().databaseText();
        qWarning() << "Driver text: " << q.lastError().driverText();
        return;
    }
}

QString DbManager::getDbPath() const
{
    return dbPath;
}

void DbManager::setDbPath(const QString &newDbPath)
{
    if (dbPath == newDbPath)
        return;
    dbPath = newDbPath;
    emit dbPathChanged();
}

void DbManager::initialize()
{
    openDb(dbPath);
//    readJsonTest();
//    readAllJsonMetadata();
}

QString DbManager::getRecordingsJsonPath() const
{
    return recordingsJsonPath;
}

void DbManager::setRecordingsJsonPath(const QString &newRecordingsJsonPath)
{
    if (recordingsJsonPath == newRecordingsJsonPath)
        return;
    recordingsJsonPath = newRecordingsJsonPath;
    emit recordingsJsonPathChanged();
}

#include "dbmanager.h"
#include <QFile>

#include <QSqlQuery>
#include <QSqlDriver>
#include <QSqlRecord>
#include <QSqlField>
#include <QSqlError>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

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
    if (!db.open())
        qDebug() << "DbManager::DbManager(): error: failed to open " << path;
    QStringList tables = db.tables();
//    QVector<QSqlRecord *> records;
    for (auto table : tables) {
        QSqlRecord record = db.record(table);;
        qDebug() << "Table " << table << ", " << record.count() << " fields";
        for (int i = 0; i < record.count(); i++) {
            QSqlField field = record.field(i);
            qDebug() << "Name: " << field.name() << "; type: " << field.type();
        }

    }

    QSqlQuery q(QSqlDatabase::database(sqlConnectionName));
    q.exec("select * from videos");

    while(q.next()) {
        qDebug() << q.value(0) << q.value(1) << q.value(2);
    }
    int x = 0;
//    db.close();
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
    for (auto &key : object.keys()) {

    }

    int x = 0;
}

void DbManager::syncJsonMetadata()
{
    RecordingMetadata metadata;
    readMetadata(R"()",
                 metadata);

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
        }
    } else
        qWarning("DbManager::syncJsonMetadata: error: empty model name");

    // Inserting the recording metadata into the "recordings" table
    if (!metadata.modelData.name.isEmpty()) {
        QSqlQuery q(db);
        q.prepare(QString("INSERT OR IGNORE INTO recordings_test (video_path, contactsheet_path, metadata_path, generated_id, last_size_update, model_name,"
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
        }
    } else
        qWarning("DbManager::syncJsonMetadata: error: recording with no video file path");

}

void DbManager::readMetadata(const QString &metadataJsonPath, RecordingMetadata &metadata)
{
    QFile file(metadataJsonPath);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString text = file.readAll();
    file.close();
    qDebug() << text;
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
    syncJsonMetadata();
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

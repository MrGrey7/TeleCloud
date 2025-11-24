#pragma once

#include <QObject>
#include <QSqlDatabase>
#include <QSet>
#include <QVector>
#include "types.h"

class DbManager : public QObject
{
    Q_OBJECT
    // Properties are excellent for QML, but if only C++, getters/setters are enough. Keeping them is fine.
    Q_PROPERTY(QString dbPath READ getDbPath WRITE setDbPath NOTIFY dbPathChanged FINAL)
    Q_PROPERTY(QString recordingsJsonPath READ getRecordingsJsonPath WRITE setRecordingsJsonPath NOTIFY recordingsJsonPathChanged FINAL)

public:
    explicit DbManager(QObject *parent = nullptr);

    QString getRecordingsJsonPath() const;
    void setRecordingsJsonPath(const QString &newRecordingsJsonPath);

    QString getDbPath() const;
    void setDbPath(const QString &newDbPath);

    void initialize();

public slots:
    void processMessage(const GenericMessage &message); // const ref
    void writeUploadToDb(const RecordingUploadInfo &upload); // const ref

signals:
    void recordingsJsonPathChanged();
    void dbPathChanged();
    void readMetadataProgressChanged(int current, int total);
    void loadedRecordingsToUpload(QVector<RecordingToUpload> uploads);

private:
    bool openDb(const QString &path);
    bool createDatabase(const QString &dbName);
    void readJsonTest();
    void readAllJsonMetadata();
    bool writeRecordingToDb(const RecordingMetadata &metadata);
    void readMetadata(const QString &metadataJsonPath, RecordingMetadata &metadata);
    void fillQueueWithRecordings();
    QString generateCaption(const QString &channelName, qint64 date);
    void updateFileStatus();
    QSet<QString> getAllJsonsFromDb();

    QString m_dbPath; // Standard member prefix "m_" helps distinguish local vars from members
    QString m_recordingsJsonPath;
};

#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QObject>
#include <QSqlDatabase>

#include <types.h>

class DbManager : public QObject
{
    Q_OBJECT
public:
    explicit DbManager(QObject *parent = nullptr);


    QString getRecordingsJsonPath() const;
    void setRecordingsJsonPath(const QString &newRecordingsJsonPath);

    QString getDbPath() const;
    void setDbPath(const QString &newDbPath);

    void initialize();

private:
    bool openDb(const QString &path);
    void readJsonTest();
    void readAllJsonMetadata();
    bool writeRecordingToDb(RecordingMetadata &metadata);
    void readMetadata(const QString &metadataJsonPath, RecordingMetadata &metadata);
    void fillQueueWithRecordings();
    QString generateCaption(QString modelName, qint64 date);
    void updateFileStatus();

private:
    QString dbPath;
    QString recordingsJsonPath;

    Q_PROPERTY(QString dbPath READ getDbPath WRITE setDbPath NOTIFY dbPathChanged FINAL)
    Q_PROPERTY(QString recordingsJsonPath READ getRecordingsJsonPath WRITE setRecordingsJsonPath NOTIFY recordingsJsonPathChanged FINAL)

public slots:
    void processMessage(GenericMessage message);
    void writeUploadToDb(RecordingUploadInfo upload);

signals:
    void recordingsJsonPathChanged();
    void dbPathChanged();
    void readMetadataProgressChanged(int, int);
    void loadedRecordingsToUpload(QVector<RecordingToUpload> uploads);
};

#endif // DBMANAGER_H

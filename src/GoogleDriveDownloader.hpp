/*
 * GoogleDriveDownloader.hpp
 *
 *  Created on: 03 мая 2015 г.
 *      Author: Sergey
 */

#ifndef GOOGLEDRIVEDOWNLOADER_HPP_
#define GOOGLEDRIVEDOWNLOADER_HPP_

#include <QObject>
#include <QQueue>
#include <QFile>

#include "GoogleFiles.hpp"
#include "RequestManager.hpp"
#include "SessionManager.hpp"
#include "FileMetaData.hpp"

class GoogleDriveDownloader: public RequestManager {
    Q_OBJECT

    Q_PROPERTY(QString url READ url WRITE setUrl NOTIFY urlChanged)
//    Q_PROPERTY(QString mimeType READ mimeType WRITE setMimeType NOTIFY mimeTypeChanged)
    Q_PROPERTY(QString filename READ filename WRITE setFilename NOTIFY filenameChanged)
//    Q_PROPERTY(QVariantMap fileMetaData READ fileMetaData WRITE setfileMetaData NOTIFY fileMetaDataChanged)
    Q_PROPERTY(bool recursion READ recursion WRITE setRecursion)
    Q_PROPERTY(bool batch READ isBatch)
public:
    GoogleDriveDownloader(QObject * parent = 0);

    Q_INVOKABLE void setProperty(const QString& name, const QVariant &value);
    Q_INVOKABLE QVariant property(const QString& name);
    Q_INVOKABLE void addToQueue(const QVariantMap& data);
    Q_INVOKABLE void downloadTo(const QString &dest);
    Q_INVOKABLE void downloadToTemp();
    Q_INVOKABLE void continueDownload(int exportIndex = -1, bool rememberChoose = false);
    Q_INVOKABLE void abort();

    QString url() const;
    void setUrl(const QString& url);
    QString filename() const;
    void setFilename(const QString& filename);
//    QString mimeType() const;
    void setMimeType(const QString &type);
    void setRecursion(bool ok);
    bool recursion();
//    QVariantMap fileMetaData() const;
//    void setfileMetaData(const QVariantMap& data);
    bool isBatch();

Q_SIGNALS:
    void urlChanged(const QString& url);
    void fileMetaDataChanged(const QVariantMap& data);
    void fileMetaDataChanged(const FileMetaData& obj);
    void mimeTypeChanged(const QString &type);
    void filenameChanged(const QString& filename);
    void downloadStarted(const QString& filename);
    void downloadCompleted(const QUrl& localfile, const QString& mimeType);
    void batchDownloadCompleted(const QUrl& destPath);
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void error(const QString &errorText);
    void chooseExportFileFormart(const QStringList& formats);


protected Q_SLOTS:
    void onReadyRead();
    void onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void onFinished();
    void onListCompleted(QList<FileMetaData> const files);

private:
    void processNextDirectory();
    void addToQueue(const FileMetaData &fileData, const QString &path);
    void processDownloadNext();

    bool _batch;

    QString _url;
    QString _mimeType;
    QString _filename;
    FileMetaData _fileMetaData;
    bool _recursion;

    QString destFolder;
    QString currentDirPath;
    QString currentLocalDirPath;
    QString _currentDownloadFilePath;
    FileMetaData _currentFileMetaData;
    QQueue< QPair<FileMetaData, QString> > _dirQueue;
    QQueue< QPair<FileMetaData, QString> > _downloadQueue;
    QQueue<FileMetaData> _queue;

    QFile destFile;
    QNetworkReply *reply;
    SessionManager *sessionManager;
    GoogleFiles *googleFiles;
    quint64 _bytesTotal;
    quint64 _bytesReceived;
    quint64 _tmpBytesReceived;
    QVariantMap _props;
};

#endif /* GOOGLEDRIVEDOWNLOADER_HPP_ */

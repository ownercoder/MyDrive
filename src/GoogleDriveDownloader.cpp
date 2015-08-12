/*
 * GoogleDriveDownloader.cpp
 *
 *  Created on: 03 мая 2015 г.
 *      Author: Sergey
 */

#include <QNetworkReply>
#include <QDir>
#include <QDebug>

#include "ApplicationSettings.hpp"
#include "FileMetaData.hpp"
#include "GoogleDriveDownloader.hpp"

GoogleDriveDownloader::GoogleDriveDownloader(QObject * parent)
        : RequestManager(parent) {
    sessionManager = new SessionManager(parent);
    setSessionManager(sessionManager);
    googleFiles = new GoogleFiles(sessionManager, this);
    connect(googleFiles, SIGNAL(listCompleted(QList<FileMetaData> const)), this, SLOT(onListCompleted(QList<FileMetaData> const)));
    _recursion = false;
    _batch = false;
}

void GoogleDriveDownloader::addToQueue(const QVariantMap& data) {
    FileMetaData fileMetaData(data);
    _queue.enqueue(fileMetaData);
}

void GoogleDriveDownloader::downloadTo(const QString &dest) {
    _bytesReceived = 0;
    _bytesTotal = 0;

    destFolder = dest;
    destFolder.remove(QRegExp("\\/$"));

    while(_queue.length()) {
        FileMetaData fileMetaData = _queue.dequeue();

        if (fileMetaData.isFolder) {
            QPair<FileMetaData, QString> item;
            item.first = fileMetaData;
            item.second = destFolder + QDir::separator() + fileMetaData.title;
            _dirQueue.enqueue(item);
        } else {
            addToQueue(fileMetaData, destFolder);
        }
    }

    processNextDirectory();
}

void GoogleDriveDownloader::downloadToTemp() {
    destFolder = QDir::tempPath();

    downloadTo(destFolder);
}

void GoogleDriveDownloader::continueDownload(int exportIndex, bool rememberChoose) {
    if (_currentFileMetaData.isEmpty()) {
        emit error("Empty file meta data");
    }

    ApplicationSettings appSettings;

    if (appSettings.getExportFormat(_currentFileMetaData.mimeType).length()) {
        for(int i = 0; i < _currentFileMetaData.exportLinks.keys().length(); i++) {
            if (_currentFileMetaData.exportLinks.keys().at(i) == appSettings.getExportFormat(_currentFileMetaData.mimeType)) {
                exportIndex = i;
                break;
            }
        }
    }

    if (exportIndex == -1 && _currentFileMetaData.exportLinks.count()) {
        emit chooseExportFileFormart(_currentFileMetaData.exportLinks.keys());
        return;
    }

    if (exportIndex > -1) {
        if (rememberChoose) {
            appSettings.rememberExportFormat(_currentFileMetaData.mimeType, _currentFileMetaData.exportLinks.keys().at(exportIndex));
        }

        _currentFileMetaData.mimeType = _currentFileMetaData.exportLinks.keys().at(exportIndex);
        QString downloadUrl = _currentFileMetaData.exportLinks.value(_currentFileMetaData.mimeType).toString();
        _currentDownloadFilePath.remove(QRegExp("\\.$"));
        _currentDownloadFilePath += "." + QUrl(downloadUrl).encodedQueryItemValue("exportFormat");
        reply = request(downloadUrl, SessionManager().getCurrentSession());
    } else {
        reply = request(_currentFileMetaData.downloadUrl, SessionManager().getCurrentSession());
    }

    if (!QFileInfo(_currentDownloadFilePath).dir().exists()) {
        QDir().mkpath(QFileInfo(_currentDownloadFilePath).dir().absolutePath());
    }

    connect(reply, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(onDownloadProgress(qint64,qint64)));
    connect(reply, SIGNAL(finished()), this, SLOT(onFinished()));
    emit downloadStarted(_currentFileMetaData.title);

    destFile.setFileName(_currentDownloadFilePath);

    if (!destFile.open(QIODevice::WriteOnly)) {
        qDebug() << tr("Cannot create file: %1").arg(destFile.fileName());
        emit error(tr("Cannot create file: %1").arg(destFile.fileName()));
        return;
    }
}

void GoogleDriveDownloader::setRecursion(bool ok) {
    _recursion = ok;
}

bool GoogleDriveDownloader::recursion() {
    return _recursion;
}

void GoogleDriveDownloader::processNextDirectory() {
    if (_dirQueue.length()) {
        QPair<FileMetaData, QString> pair = _dirQueue.dequeue();
        FileMetaData _currentDir = pair.first;
        currentDirPath = pair.second;

        currentLocalDirPath = currentDirPath;

        googleFiles->list(_currentDir.id);
    } else {
        processDownloadNext();
    }
}

void GoogleDriveDownloader::addToQueue(const FileMetaData &fileData, const QString &path) {
    QPair<FileMetaData, QString> item;
    item.first = fileData;
    item.second = path + QDir::separator() + fileData.title;
    _downloadQueue.enqueue(item);
    _batch = _downloadQueue.length() > 1;
    _bytesTotal += fileData.fileSize;
}

void GoogleDriveDownloader::processDownloadNext() {
    if (_downloadQueue.length()) {
        QPair<FileMetaData, QString> item = _downloadQueue.dequeue();
        _currentFileMetaData = item.first;
        _currentDownloadFilePath = item.second;
        continueDownload();
    } else {
        if (!_batch) {
            emit downloadCompleted(destFile.fileName(), _currentFileMetaData.mimeType);
        } else if (!_downloadQueue.length()) {
            emit batchDownloadCompleted(destFolder);
        }
    }
}

void GoogleDriveDownloader::onListCompleted(QList<FileMetaData> const files) {
    for(int i = 0;i < files.length();i++) {
        if (files.at(i).isFolder) {
            if (!_recursion)
                continue;

            QPair<FileMetaData, QString> item;
            item.first = files.at(i);
            item.second = currentDirPath + QDir::separator() + files.at(i).title;
            _dirQueue.enqueue(item);
        } else {
            addToQueue(files.at(i), currentLocalDirPath);
        }
    }

    processNextDirectory();
}

void GoogleDriveDownloader::abort() {
    _bytesReceived = 0;
    _bytesTotal = 0;
    _downloadQueue.clear();
    _dirQueue.clear();
    _queue.clear();
    if (reply)
        reply->abort();
}

/*QVariantMap GoogleDriveDownloader::fileMetaData() const {
    return _fileMetaData.toMap();
}

void GoogleDriveDownloader::setfileMetaData(const QVariantMap& data) {
    _fileMetaData = FileMetaData(data);
}

QString GoogleDriveDownloader::mimeType() const {
    return _fileMetaData.mimeType;
}*/

void GoogleDriveDownloader::setProperty(const QString& name, const QVariant &value) {
    _props.remove(name);
    _props.insert(name, value);
}

QVariant GoogleDriveDownloader::property(const QString& name) {
    return _props.value(name);
}

bool GoogleDriveDownloader::isBatch() {
    return _batch;
}


void GoogleDriveDownloader::setMimeType(const QString& type) {
    _mimeType = type;
}

QString GoogleDriveDownloader::filename() const {
    return _filename;
}

void GoogleDriveDownloader::setFilename(const QString& filename) {
    _filename = filename;
}

void GoogleDriveDownloader::onFinished() {
    destFile.close();
    _bytesReceived += _tmpBytesReceived;
    processDownloadNext();
}

void GoogleDriveDownloader::onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal) {
    _tmpBytesReceived = bytesReceived;
    Q_UNUSED(bytesTotal);

    if (!_bytesTotal) {
        emit downloadProgress(_bytesReceived + bytesReceived, bytesTotal);
        qDebug() << "Progress " << _bytesReceived + bytesReceived << bytesTotal;
    } else {
        qDebug() << "Progress " << _bytesReceived + bytesReceived << _bytesTotal;
        emit downloadProgress(_bytesReceived + bytesReceived, _bytesTotal);
    }
}

void GoogleDriveDownloader::onReadyRead() {
    if (reply->bytesAvailable()) {
        destFile.write(reply->readAll());
    }
}

QString GoogleDriveDownloader::url() const {
    return _url;
}

void GoogleDriveDownloader::setUrl(const QString& url) {
    _url = url;
}

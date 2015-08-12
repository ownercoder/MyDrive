/*
 * GoogleDriveUploader.cpp
 *
 *  Created on: 13 июня 2015 г.
 *      Author: Sergey
 */
#include <QHttpMultiPart>
#include <QHttpPart>
#include <QFile>
#include <QFileInfo>
#include <bb/data/JsonDataAccess>
#include "GoogleDriveUploader.hpp"

GoogleDriveUploader::GoogleDriveUploader(QObject * parent) :
    RequestManager(parent)
{
    currentUploadPath = "";
    currentFilePath = "";
    currentFileName = "";
    totalBytesForSend = 0;
    totalBytesSent = 0;
}

void GoogleDriveUploader::onUploadProgress(qint64 bytesSent, qint64 bytesTotal)
{
    emit uploadProgress(totalBytesSent + bytesSent, totalBytesForSend, currentFileName);
    qDebug() << totalBytesSent + bytesSent << bytesTotal;
}

void GoogleDriveUploader::uploadTo(const QString &uploadPath)
{
    if (!_queue.length()) {
        return;
    }

    QString filePath = _queue.dequeue();

    QFile *file = new QFile(filePath);
    if (!file->open(QIODevice::ReadOnly)) {
        qDebug() << file->errorString();
        return;
    }

    currentFilePath = filePath;
    currentUploadPath = uploadPath;
    currentFileName = QFileInfo(currentFilePath).fileName();

    QByteArray fileMetaData;

    QVariantMap jsonData;
    jsonData.insert("title", currentFileName);
    QVariantMap parents;
    parents.insert("id", uploadPath);
    QVariantList ids;
    ids.append(parents);
    jsonData.insert("parents", ids);

    bb::data::JsonDataAccess jda;
    jda.saveToBuffer(jsonData, &fileMetaData);

    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::RelatedType);

    QHttpPart metaDataPart;
    metaDataPart.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    metaDataPart.setBody(fileMetaData);

    QHttpPart filePart;
    filePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/octet-stream"));
    filePart.setBodyDevice(file);
    file->setParent(multiPart);

    multiPart->append(metaDataPart);
    multiPart->append(filePart);

    QUrl url("https://www.googleapis.com/upload/drive/v2/files?uploadType=multipart");
    QNetworkRequest request(url);
    request.setRawHeader("Authorization", QString(SessionManager().getCurrentSession().tokenType +  " " + SessionManager().getCurrentSession().token).toUtf8());

    reply = post(request, multiPart);

    emit uploadStarted();

    connect(reply, SIGNAL(uploadProgress(qint64,qint64)), this, SLOT(onUploadProgress(qint64,qint64)));
    connect(reply, SIGNAL(finished()), this, SLOT(onFinished()));
    multiPart->setParent(reply);
}

void GoogleDriveUploader::onFinished()
{
    reply->deleteLater();

    totalBytesSent += QFileInfo(currentFilePath).size();

    if (_queue.length()) {
        uploadTo(currentUploadPath);
    } else {
        currentUploadPath = "";
        currentFilePath = "";
        currentFileName = "";
        totalBytesForSend = 0;
        totalBytesSent = 0;
        emit uploadCompleted();
    }
}

void GoogleDriveUploader::addToQueue(const QString& path)
{
    _queue.enqueue(path);
    totalBytesForSend = totalBytesForSend + QFileInfo(path).size();
}

void GoogleDriveUploader::abort()
{
    _queue.clear();

    if (reply) {
        reply->abort();
        reply->deleteLater();
    }

    currentUploadPath = "";
    currentFilePath = "";
    currentFileName = "";
    totalBytesForSend = 0;
    totalBytesSent = 0;

    emit uploadCompleted();
}

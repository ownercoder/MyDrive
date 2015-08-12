/*
 * ApiWrapper.cpp
 *
 *  Created on: 11 мая 2015 г.
 *      Author: Sergey
 */
#include <bb/data/JsonDataAccess.hpp>
#include "ApiWrapper.hpp"

using namespace bb::data;

ApiWrapper::ApiWrapper(QObject * parent) : RequestManager(parent) {
    sessM.setTokenManager(&tokenManager);
    setSessionManager(&sessM);
}

ApiWrapper::~ApiWrapper() {
}

void ApiWrapper::addToQueue(const QVariantMap &data) {
    _queue.enqueue(FileMetaData(data));
}

void ApiWrapper::trash() {
    emit requestStarted();
    processTrash();
}

void ApiWrapper::createDirectory(const QString &name, const QString &parent)
{
    emit requestStarted();

    QVariantMap fileMetaData;
    fileMetaData.insert("title", name);
    fileMetaData.insert("mimeType", "application/vnd.google-apps.folder");

    QVariantList parentIds;
    QVariantMap ids;
    ids.insert(QString("id"), parent);
    parentIds.append(ids);
    fileMetaData.insert("parents", parentIds);

    QByteArray jsonData;
    JsonDataAccess jda;
    jda.saveToBuffer(fileMetaData, &jsonData);

    QMultiMap<QString, QString> requestData;
    requestData.insert(QString("post_data"), jsonData);

    qDebug() << "JSON DATA: " << jsonData;

    reply = request("https://www.googleapis.com/drive/v2/files", sessM.getCurrentSession(), requestData, RequestManager::MethodPOST);
    reply->setProperty("type", "createDirectory");
    connect(reply, SIGNAL(finished()), this, SLOT(onFinished()));
}

void ApiWrapper::processTrash() {
    if (!_queue.length()) {
        emit requestFinished();
        return;
    }

    FileMetaData fileMetaData = _queue.dequeue();
    QMultiMap<QString, QString> requestData;

    reply = request(QString("https://www.googleapis.com/drive/v2/files/%1/trash").arg(fileMetaData.id), sessM.getCurrentSession(), requestData, RequestManager::MethodPOST);
    reply->setProperty("type", "processTrash");
    connect(reply, SIGNAL(finished()), this, SLOT(onFinished()));
}

void ApiWrapper::abort() {
    _queue.clear();

    if (reply) {
        reply->abort();
    }
}

void ApiWrapper::onFinished() {
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << reply->errorString();
        emit error(reply->errorString());
        return;
    }

    if (reply->property("type").toString() == "processTrash") {
        reply->deleteLater();
        return processTrash();
    }

    if (reply->property("type").toString() == "createDirectory") {
        JsonDataAccess jda;
        QVariant data = jda.load(reply);
        QVariantMap map = data.value<QVariantMap>();

        FileMetaData folder(map);
        emit folderCreated(folder.toMap());
    }

    emit requestFinished();
    reply->deleteLater();
}

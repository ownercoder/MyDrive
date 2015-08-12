/*
 * GoogleFiles.cpp
 *
 *  Created on: 25 апр. 2015 г.
 *      Author: Sergey
 */

#include <QVariantMap>
#include <QDebug>
#include <QNetworkReply>
#include <bb/data/JsonDataAccess>
#include <QEventLoop>

#include <src/GoogleFiles.hpp>

GoogleFiles::GoogleFiles(SessionManager *sessionManager, QObject * parent) :
        RequestManager(parent)
        , sessM(sessionManager) {
    setSessionManager(sessM);
}

void GoogleFiles::list(const QString &parent, GoogleFolder folderType, const QString &pageToken) {
    QMultiMap<QString, QString> queryItems;
    switch(folderType) {
        case GoogleFiles::MyFolder:
            queryItems.insert("q", "'" + parent + "' in parents and trashed = false");
            break;
        case GoogleFiles::SharedWithMe:
            queryItems.insert("q", "sharedWithMe = true and trashed = false");
            break;
        case GoogleFiles::Starred:
            queryItems.insert("q", "trashed = false and starred = true");
            break;
        case GoogleFiles::Trashed:
            queryItems.insert("q", "trashed = true");
            break;
        case GoogleFiles::NoFolder:
            queryItems.insert("q", "'" + parent + "' in parents and trashed = false");
            break;
        default:
            return;
    }
    queryItems.insert("fields", "items(id,title,mimeType,thumbnailLink,parents/id,folderColorRgb,downloadUrl,exportLinks,fileSize,labels,shared,writersCanShare,modifiedDate),nextPageToken");
    if (pageToken.length()) {
        queryItems.insert("pageToken", pageToken);
    }

    reply = request(QString("https://www.googleapis.com/drive/v2/files"), SessionManager().getCurrentSession(), queryItems, RequestManager::MethodGET);
    reply->setProperty("parentId", parent);
    reply->setProperty("pageToken", pageToken);
    reply->setProperty("folderType", folderType);
    connect(reply, SIGNAL(finished()), this, SLOT(requestListFinished()), Qt::QueuedConnection);
}

void GoogleFiles::requestListFinished() {
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << reply->errorString() << reply->error();
        if (reply->error() == QNetworkReply::AuthenticationRequiredError) {
            QEventLoop loop;
            sessM->refreshExpiredSessions();
            connect(sessM, SIGNAL(sessionRefreshCompleted()), &loop, SLOT(quit()));
            loop.exec();
            list(reply->property("parentId").toString(), static_cast<GoogleFolder>(reply->property("folderType").toInt()), reply->property("pageToken").toString());
            return;
        }

        files.clear();
        emit listCompleted(files);
    }

    bb::data::JsonDataAccess jda;
    QVariant data = jda.load(reply);

    QVariantList rawFiles = data.toMap().value("items").value<QVariantList>();
    QString nextPageToken = data.toMap().value("nextPageToken").toString();
    for(int i = 0;i<rawFiles.length();i++) {
        QVariantMap map = rawFiles.value(i).value<QVariantMap>();
        files.append(FileMetaData(map));
    }

    if (nextPageToken.length()) {
        list(reply->property("parentId").toString(), static_cast<GoogleFolder>(reply->property("folderType").toInt()), nextPageToken);
    } else {
        emit listCompleted(files);
        files.clear();
    }
}

void GoogleFiles::search(const QString& filename, const QString& pageToken, int maxItems) {
    QMultiMap<QString, QString> queryItems;
    queryItems.insert("q", "title contains '" + filename + "' and trashed = false");
    queryItems.insert("fields", "items(id,title,mimeType,thumbnailLink,parents/id,folderColorRgb,downloadUrl,exportLinks,fileSize,labels,shared,writersCanShare,modifiedDate),nextPageToken");
    queryItems.insert("maxResults", QString::number(maxItems));
    if (pageToken.length()) {
        queryItems.insert("pageToken", pageToken);
    }

    reply = request(QString("https://www.googleapis.com/drive/v2/files"), SessionManager().getCurrentSession(), queryItems, RequestManager::MethodGET);
    reply->setProperty("filename", filename);
    reply->setProperty("pageToken", pageToken);
    reply->setProperty("maxItems", maxItems);
    connect(reply, SIGNAL(finished()), this, SLOT(requestSearchFinished()));
}

void GoogleFiles::requestSearchFinished() {
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << reply->errorString() << reply->error();
        if (reply->error() == QNetworkReply::AuthenticationRequiredError) {
            QEventLoop loop;
            sessM->refreshExpiredSessions();
            connect(sessM, SIGNAL(sessionRefreshCompleted()), &loop, SLOT(quit()));
            loop.exec();
            search(reply->property("filename").toString(), reply->property("pageToken").toString(), reply->property("maxItems").toInt());
            return;
        }

        files.clear();
        emit searchCompleted(files, "");
    }

    bb::data::JsonDataAccess jda;
    QVariant data = jda.load(reply);
    qDebug() << reply;

    QVariantList rawFiles = data.toMap().value("items").value<QVariantList>();
    QString nextPageToken = data.toMap().value("nextPageToken").toString();
    for(int i = 0;i<rawFiles.length();i++) {
        QVariantMap map = rawFiles.value(i).value<QVariantMap>();
        qDebug() << map;
        files.append(FileMetaData(map));
    }

    emit searchCompleted(files, nextPageToken);
    files.clear();
}

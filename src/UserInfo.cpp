/*
 * UserInfo.cpp
 *
 *  Created on: 09 мая 2015 г.
 *      Author: Sergey
 */

#include <QEventLoop>
#include <bb/data/JsonDataAccess>
#include <QDebug>

#include "UserInfo.hpp"

UserInfo::UserInfo(QObject * parent) : RequestManager(parent) {
    TokenManager *token = new TokenManager(this);
    sessM = new SessionManager(this);
    sessM->setTokenManager(token);
}

void UserInfo::getUserDataInfo(QList<GoogleSession> sessions) {
    userInfos.clear();

    for(int i=0;i<sessions.length();i++) {
        queue.enqueue(sessions.at(i));
    }

    getNext();
}

void UserInfo::getNext() {
    if (queue.length() == 0) {
        emit infoReceived(userInfos);
        return;
    }
    currentRequestSession = queue.dequeue();
    QMultiMap<QString, QString> queryItems;
    queryItems.insert("fields", "quotaBytesUsed,folderColorPalette,quotaBytesTotal,quotaBytesUsedAggregate,quotaBytesUsedInTrash,user");
    reply = request("https://www.googleapis.com/drive/v2/about", currentRequestSession, queryItems);
    connect(reply, SIGNAL(finished()), this, SLOT(onFinished()));
}

void UserInfo::onFinished() {
    disconnect(reply, SIGNAL(finished()), this, SLOT(onFinished()));
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << reply->errorString() << reply->error();
        if (reply->error() == QNetworkReply::AuthenticationRequiredError) {
            QEventLoop loop;
            sessM->refreshSession(currentRequestSession);
            connect(sessM, SIGNAL(sessionRefreshCompleted()), &loop, SLOT(quit()));
            loop.exec();
            queue.enqueue(sessM->getSessionById(currentRequestSession.uniqId));
            getNext();
            return;
        }
        emit infoReceived(userInfos);
        return;
    }

    bb::data::JsonDataAccess jda;
    QVariant data = jda.load(reply);

    userInfos.append(UserInfoData(data.toMap(), currentRequestSession.uniqId));

    reply->deleteLater();
    getNext();
}

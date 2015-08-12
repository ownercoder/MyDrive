/*
 * RequestManager.cpp
 *
 *  Created on: 25 апр. 2015 г.
 *      Author: Sergey
 */

#include <QUrl>
#include <QNetworkRequest>
#include <QEventLoop>
#include <bb/data/JsonDataAccess>
#include "RequestManager_p.hpp"
#include "RequestManager.hpp"

// Private implementation
RequestManagerPrivate::RequestManagerPrivate(RequestManager * parent) :
    q_ptr(parent) {

}

RequestManagerPrivate::~RequestManagerPrivate() {
    delete q_ptr;
}

// END - Private implementation

RequestManager::RequestManager(QObject * parent) :
        QNetworkAccessManager(parent)
        , d_ptr(new RequestManagerPrivate(this)) {
}

void RequestManager::setSessionManager(SessionManager *sessionManager) {
    sessM = sessionManager;
}

QNetworkReply* RequestManager::request(const QString &url, GoogleSession session, QMultiMap<QString, QString> requestData, RequestMethod method) {
    QUrl requestUrl(url);

    QList<QString> keys = requestData.keys();
    QByteArray postData;

    for(int i = 0;i<keys.length();i++) {
        if (keys.at(i) == "post_data") {
            postData.append(requestData.value(keys.at(i)));
        } else {
            requestUrl.addQueryItem(keys.at(i), requestData.value(keys.at(i)));
        }
    }

    QNetworkRequest request(requestUrl);
    request.setRawHeader("Authorization", QString(session.tokenType +  " " + session.token).toUtf8());

    switch(method) {
        case RequestManager::MethodGET:
            return get(request);
            break;
        case RequestManager::MethodPOST:
            qDebug() << qPrintable(postData);
            request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
            request.setHeader(QNetworkRequest::ContentLengthHeader, postData.size());
            return post(request, postData);
            break;
    }

    return NULL;
}

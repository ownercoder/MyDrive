/*
 * TokenRequest.cpp
 *
 *  Created on: 22 марта 2015 г.
 *      Author: Sergey
 */

#include <QDebug>
#include <QDateTime>
#include <QVariantMap>
#include <QCryptographicHash>

#include "TokenManager.hpp"
#include "TokenManager_p.hpp"
#include "utils.hpp"

// Private implementation
const char * TokenManagerPrivate::_grantTypes[] = { "authorization_code", "refresh_token" };
const QString TokenManagerPrivate::TOKEN_URL    = "https://www.googleapis.com/oauth2/v3/token";

TokenManagerPrivate::TokenManagerPrivate(TokenManager * parent) :
        QObject(parent)
        , q_ptr(parent) {

}

TokenManagerPrivate::~TokenManagerPrivate() {
    q_ptr->deleteLater();
}

void TokenManagerPrivate::request(QUrl postData, GrantTypes grantType) {
    Q_Q(TokenManager);

    QString grantTypeString = flagsToStrList(grantType, _grantTypes).at(0);

    postData.addQueryItem("client_id", clientId);
    postData.addQueryItem("client_secret", clientSecret);
    postData.addQueryItem("grant_type", grantTypeString);

    QNetworkRequest request(TokenManagerPrivate::TOKEN_URL);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    qDebug() << "Post data: " << postData.encodedQuery();
    currentGrantType = grantType;
    q->post(request, postData.encodedQuery());
}

void TokenManagerPrivate::onFinished(QNetworkReply * reply) {
    JsonDataAccess jda;
    QVariant list = jda.load(reply);

    QVariantMap variantMap = list.value<QVariantMap>();

    Q_Q(TokenManager);

    if (variantMap.value("error").toString().length()) {
        qDebug() << variantMap.value("error").toString() << variantMap.value("error_description").toString();
        return;
    }

    if (currentGrantType == TokenManagerPrivate::AuthorizationCode) {
        GoogleSession session;
        session.token = variantMap.value("access_token").toString();
        session.tokenType = variantMap.value("token_type").toString();
        session.tokenExpire = QDateTime::currentDateTime().toTime_t() + variantMap.value("expires_in").toUInt();
        session.refreshToken = variantMap.value("refresh_token").toString();
        session.uniqId = QCryptographicHash::hash(QByteArray::number(QDateTime::currentDateTime().toTime_t()) + session.token.toUtf8(), QCryptographicHash::Md5).toHex();
        qDebug() << "Uniq id: " << session.uniqId;

        emit q->sessionCreated(session);
    } else if (currentGrantType == TokenManagerPrivate::RefreshToken) {
        refreshSession.token = variantMap.value("access_token").toString();
        refreshSession.tokenType = variantMap.value("token_type").toString();
        refreshSession.tokenExpire = QDateTime::currentDateTime().toTime_t() + variantMap.value("expires_in").toUInt();
        emit q->sessionRefreshed(refreshSession);
    }
}
// END Private implementation

TokenManager::TokenManager(QObject * parent) :
        QNetworkAccessManager(parent)
        , d_ptr(new TokenManagerPrivate(this)) {
    Q_D(TokenManager);

    connect(this, SIGNAL(finished(QNetworkReply*)), d, SLOT(onFinished(QNetworkReply*)));
}

void TokenManager::setRedirectUrl(const QUrl &url) {
    Q_D(TokenManager);

    d->redirectUri = url;
}

void TokenManager::setClientSecret(const QString &clientSecret) {
    Q_D(TokenManager);

    d->clientSecret = clientSecret;
}

void TokenManager::setClientId(const QString &clientId) {
    Q_D(TokenManager);

    d->clientId = clientId;
}


void TokenManager::getToken(const QString &code) {
    Q_D(TokenManager);

    QUrl postData;
    postData.addQueryItem("code", code);
    postData.addQueryItem("redirect_uri", d->redirectUri.toString());

    d->request(postData, TokenManagerPrivate::AuthorizationCode);
}

void TokenManager::refreshSession(const GoogleSession &session) {
    Q_D(TokenManager);

    d->refreshSession = session;
    QUrl postData;
    postData.addQueryItem("refresh_token", session.refreshToken);

    d->request(postData, TokenManagerPrivate::RefreshToken);
}

TokenManager::~TokenManager()
{
    d_ptr->deleteLater();
}


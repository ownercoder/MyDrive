/*
 * SessionManager.cpp
 *
 *  Created on: 22 марта 2015 г.
 *      Author: Sergey
 */

#include <QUrl>
#include <QDateTime>

#include "SessionManager_p.hpp"
#include "SessionManager.hpp"
#include "utils.hpp"

// Private implementation
const char * SessionManagerPrivate::_scopes[]       = { "https://www.googleapis.com/auth/drive", "email", "profile" };
const QString SessionManagerPrivate::AUTH_URL       = "https://accounts.google.com/o/oauth2/auth";
const QString SessionManagerPrivate::RESPONSE_TYPE  = "code";
const QString SessionManagerPrivate::CLIENT_ID      = "726983064038-hq7obdm4ljnga8d0vg9ftr333bo3jrnn.apps.googleusercontent.com";
const QString SessionManagerPrivate::CLIENT_SECRET  = "-Pf_k2LGNykSlL6YdmEj89XK";

SessionManagerPrivate::SessionManagerPrivate(SessionManager * parent) :
        q_ptr(parent)
        , callbackServer(new AuthResponseServer(parent)) {
}

SessionManagerPrivate::~SessionManagerPrivate() {

}

void SessionManagerPrivate::initResponseServer() {
    callbackServer->listen();
}

// END Private imlementation

SessionManager::SessionManager(QObject * parent) :
        QObject(parent),
        d_ptr(new SessionManagerPrivate(this)) {
}

SessionManager::~SessionManager() {

}

void SessionManager::setTokenManager(TokenManager * tokenManager) {
    Q_D(SessionManager);

    d->tokenManager = tokenManager;
    d->tokenManager->setClientId(SessionManagerPrivate::CLIENT_ID);
    d->tokenManager->setClientSecret(SessionManagerPrivate::CLIENT_SECRET);
    connect(d->tokenManager, SIGNAL(sessionRefreshed(GoogleSession)), this, SLOT(onSessionRefreshed(GoogleSession)));
}

QList<GoogleSession> SessionManager::getSessions() {
    Q_D(SessionManager);

    return d->setting.value("sessions").value< QList<GoogleSession> >();
}

void SessionManager::onVerificationReceived(QMultiMap<QString, QString> code) {
    Q_D(SessionManager);
    if (code.contains("code")) {
        qDebug() << "Request for token: " << code.value("code");
        d->tokenManager->getToken(code.value("code"));
    }
}

GoogleSession SessionManager::getSessionById(const QString &id) {
    Q_D(SessionManager);

    QList<GoogleSession> sessions = d->setting.value("sessions").value< QList<GoogleSession> >();
    for(int i = 0;i < sessions.length();i++) {
        if (id == sessions.at(i).uniqId) {
           return sessions.at(i);
        }
    }

    qDebug() << "Cannot get session by id: " << id;

    GoogleSession session;
    return session;
}

QUrl SessionManager::getAuthenticationUrl(Scopes scopes) {
    Q_D(SessionManager);

    QString scopesString = flagsToStrList(scopes, d->_scopes).join("%20");

    d->initResponseServer();

    SessionManager * ther = this;
    connect(d->callbackServer, SIGNAL(verificationReceived(QMultiMap<QString, QString>)), ther, SLOT(onVerificationReceived(QMultiMap<QString, QString>)));

    QString redirectUrl = "http://localhost:" + QString::number(d->callbackServer->serverPort());
    d->tokenManager->setRedirectUrl(redirectUrl);

    QUrl url(SessionManagerPrivate::AUTH_URL);
    url.addQueryItem("scope", scopesString);
    url.addQueryItem("redirect_uri", redirectUrl);
    url.addQueryItem("response_type", SessionManagerPrivate::RESPONSE_TYPE);
    url.addQueryItem("client_id", SessionManagerPrivate::CLIENT_ID);
    qDebug() << url;

    return url;
}

void SessionManager::removeSession(const QString &id) {
    Q_D(SessionManager);

    QList<GoogleSession> newSessionsList;
    QList<GoogleSession> sessions = d->setting.value("sessions").value< QList<GoogleSession> >();
    for(int i = 0;i < sessions.length();i++) {
        if (sessions.at(i).uniqId == id) {
            continue;
        } else {
            newSessionsList.append(sessions.at(i));
        }
    }

    d->setting.setValue("sessions", QVariant::fromValue< QList<GoogleSession> >(newSessionsList));
}

void SessionManager::storeSession(const GoogleSession& session) {
    Q_D(SessionManager);

    bool add = true;

    QList<GoogleSession> sessions = d->setting.value("sessions").value< QList<GoogleSession> >();
    for(int i = 0;i < sessions.length();i++) {
        if (sessions.at(i).uniqId == session.uniqId) {
            sessions.replace(i, session);
            add = false;
            break;
        }
    }

    if (add) {
        sessions.append(session);
    }

    d->setting.setValue("sessions", QVariant::fromValue< QList<GoogleSession> >(sessions));
}

void SessionManager::refreshExpiredSessions() {
    Q_D(SessionManager);

    QList<GoogleSession> sessions = d->setting.value("sessions").value< QList<GoogleSession> >();
    for(int i = 0;i < sessions.length();i++) {
        if (sessions.at(i).tokenExpire <= QDateTime::currentDateTime().toTime_t()) {
            d->tokenManager->refreshSession(sessions.at(i));
        }
    }
}

void SessionManager::refreshSession(GoogleSession session) {
    Q_D(SessionManager);

    d->tokenManager->refreshSession(session);
}

void SessionManager::setCurrentSession(const GoogleSession& session) {
    Q_D(SessionManager);

    d->setting.setValue("current_session", session.uniqId);
}

GoogleSession SessionManager::getCurrentSession() {
    Q_D(SessionManager);

    QString uniqId = d->setting.value("current_session").toString();
    qDebug() << "Current uniq id: " << uniqId;

    QList<GoogleSession> sessions = d->setting.value("sessions").value< QList<GoogleSession> >();
    for(int i = 0;i < sessions.length();i++) {
        qDebug() << "Session uniq id: " << sessions.at(i).uniqId;
        if (uniqId == sessions.at(i).uniqId) {
           return sessions.at(i);
        }
    }

    GoogleSession session;
    return session;
}

void SessionManager::onSessionRefreshed(GoogleSession session) {
    qDebug() << "Refreshed session received: " << session.token << session.tokenExpire;
    storeSession(session);
    emit sessionRefreshCompleted();
}

/*
 * SessionWrapper.cpp
 *
 *  Created on: 09 мая 2015 г.
 *      Author: Sergey
 */
#include "SessionWrapper.hpp"

#include <QUrl>
#include <QDebug>

#include "TokenManager.hpp"
#include "SessionManager.hpp"
#include "GoogleSession.hpp"

SessionWrapper::SessionWrapper(QObject * parent) : QObject(parent) {
    TokenManager *token = new TokenManager(parent);
    sessManager = new SessionManager(parent);

    sessManager->setTokenManager(token);
    connect(token, SIGNAL(sessionCreated(GoogleSession)), this, SLOT(onSessionCreated(GoogleSession)));
}

SessionWrapper::~SessionWrapper() {

}

void SessionWrapper::exitCurrentSession() {
    GoogleSession currentSession = sessManager->getCurrentSession();
    sessManager->removeSession(currentSession.uniqId);
}

QString SessionWrapper::getLoginUrl() {
    return sessManager->getAuthenticationUrl(SessionManager::Drive | SessionManager::Profile | SessionManager::Email).toString();
}

void SessionWrapper::changeUserAccount(const QString& id) {
    QList<GoogleSession> sessionList = sessManager->getSessions();
    for(int i=0;i<sessionList.length();i++) {
        if (sessionList.at(i).uniqId == id) {
            sessManager->setCurrentSession(sessionList.at(i));
            return;
        }
    }

    qDebug() << "Cannot change current session: " << id;
}

void SessionWrapper::onSessionCreated(GoogleSession session) {
    sessManager->storeSession(session);
    sessManager->setCurrentSession(session);
    emit userSessionCreated();
}

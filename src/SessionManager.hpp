/*
 * SessionManager.hpp
 *
 *  Created on: 22 марта 2015 г.
 *      Author: Sergey
 */

#ifndef SESSIONMANAGER_HPP_
#define SESSIONMANAGER_HPP_

#include <QObject>

#include "TokenManager.hpp"
#include "GoogleSession.hpp"

class SessionManagerPrivate;
class SessionManager: public QObject {
    Q_OBJECT
    Q_FLAGS(Scopes);
public:
    explicit SessionManager(QObject * parent = 0);
    virtual ~SessionManager();

    void setTokenManager(TokenManager * tokenManager);

    enum Scope {
        Drive        = 0x1,
        Email        = 0x2,
        Profile      = 0x4
    };

    Q_DECLARE_FLAGS(Scopes, Scope)

    QUrl getAuthenticationUrl(Scopes scopes);

    QList<GoogleSession> getSessions();
    void storeSession(const GoogleSession &session);
    void refreshExpiredSessions();
    void refreshSession(GoogleSession session);
    void setCurrentSession(const GoogleSession& session);
    void removeSession(const QString &id);
    GoogleSession getCurrentSession();
    GoogleSession getSessionById(const QString &id);
signals:
    void sessionCreated(GoogleSession session);
    void sessionRefreshCompleted();
public Q_SLOTS:
    void onVerificationReceived(QMultiMap<QString, QString> code);
    void onSessionRefreshed(GoogleSession session);

private:
    Q_DECLARE_PRIVATE(SessionManager);
    SessionManagerPrivate * const d_ptr;
};
Q_DECLARE_OPERATORS_FOR_FLAGS(SessionManager::Scopes)

#endif /* SESSIONMANAGER_HPP_ */

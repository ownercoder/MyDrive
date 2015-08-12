/*
 * SessionManager_p.hpp
 *
 *  Created on: 22 марта 2015 г.
 *      Author: Sergey
 */

#ifndef SESSIONMANAGER_P_HPP_
#define SESSIONMANAGER_P_HPP_

#include <QSettings>

#include "SessionManager.hpp"
#include "AuthResponseServer.hpp"
#include "TokenManager.hpp"


class SessionManagerPrivate
{
public:
    SessionManagerPrivate(SessionManager * parent);
    virtual ~SessionManagerPrivate();

    QSettings setting;
    static const char* _scopes[];
    static const QString AUTH_URL;
    static const QString RESPONSE_TYPE;
    static const QString CLIENT_ID;
    static const QString CLIENT_SECRET;

    void initResponseServer();

    SessionManager * const q_ptr;
    Q_DECLARE_PUBLIC(SessionManager);

    AuthResponseServer * callbackServer;
    TokenManager * tokenManager;
};

#endif /* SESSIONMANAGER_P_HPP_ */

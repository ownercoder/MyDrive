/*
 * TokenRequest.hpp
 *
 *  Created on: 22 марта 2015 г.
 *      Author: Sergey
 */

#ifndef TOKENREQUEST_HPP_
#define TOKENREQUEST_HPP_

#include <QObject>
#include <QNetworkAccessManager>

#include "GoogleSession.hpp"

class TokenManagerPrivate;
class TokenManager: public QNetworkAccessManager
{
    Q_OBJECT
public:
    explicit TokenManager(QObject * parent = 0);
    virtual ~TokenManager();

    void setClientSecret(const QString &clientSecret);
    void setClientId(const QString &clientId);
    void getToken(const QString &code);
    void setRedirectUrl(const QUrl &url);
    void refreshSession(const GoogleSession& session);

Q_SIGNALS:
    void sessionCreated(GoogleSession session);
    void sessionRefreshed(GoogleSession session);

private:
    TokenManagerPrivate * d_ptr;
    Q_DECLARE_PRIVATE(TokenManager);
};

#endif /* TOKENREQUEST_HPP_ */

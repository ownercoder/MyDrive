/*
 * TokenRequest.hpp
 *
 *  Created on: 22 марта 2015 г.
 *      Author: Sergey
 */

#ifndef TOKENMANAGER_P_HPP_
#define TOKENMANAGER_P_HPP_

#include <QNetworkReply>
#include <bb/data/JsonDataAccess>

#include "TokenManager.hpp"

using namespace bb::data;

class TokenManagerPrivate: public QObject
{
    Q_OBJECT
    Q_FLAGS(GrantTypes);
public:
    TokenManagerPrivate(TokenManager * parent);
    virtual ~TokenManagerPrivate();

    enum GrantType {
        AuthorizationCode = 0x1,
        RefreshToken      = 0x2
    };

    Q_DECLARE_FLAGS(GrantTypes, GrantType)

    void request(QUrl postData, GrantTypes grantType);

    QNetworkReply * networkReply;

    QString clientSecret;
    QString clientId;
    QUrl redirectUri;

    static const char* _grantTypes[];
    static const QString TOKEN_URL;

    GoogleSession refreshSession;
    GrantTypes currentGrantType;
public Q_SLOTS:
    void onFinished(QNetworkReply * reply);

public:
    TokenManager * const q_ptr;
    Q_DECLARE_PUBLIC(TokenManager);
};

Q_DECLARE_OPERATORS_FOR_FLAGS(TokenManagerPrivate::GrantTypes)

#endif /* TOKENMANAGER_P_HPP_ */

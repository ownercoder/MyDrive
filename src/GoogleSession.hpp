/*
 * Session.hpp
 *
 *  Created on: 22 марта 2015 г.
 *      Author: Sergey
 */

#ifndef SESSION_HPP_
#define SESSION_HPP_

#include <QObject>
#include <QString>
#include <QDataStream>

class GoogleSession {
public:
    QString token;
    QString tokenType;
    uint tokenExpire;
    QString refreshToken;
    QString uniqId;

    friend QDataStream &operator<<(QDataStream &out, const GoogleSession &t) {
        out << t.token << t.tokenExpire << t.refreshToken << t.uniqId << t.tokenType;
        return out;
    }

    friend QDataStream &operator>>(QDataStream &in, GoogleSession &t) {
        in >> t.token >> t.tokenExpire >> t.refreshToken >> t.uniqId >> t.tokenType;
        return in;
    }
};

Q_DECLARE_METATYPE(GoogleSession);
Q_DECLARE_METATYPE(QList<GoogleSession>);

#endif /* SESSION_HPP_ */

/*
 * AuthResponseServer.hpp
 *
 *  Created on: 22 марта 2015 г.
 *      Author: Sergey
 */

#ifndef AUTHRESPONSESERVER_P_HPP_
#define AUTHRESPONSESERVER_P_HPP_

#include <QMultiMap>
#include <QString>

#include "AuthResponseServer.hpp"

class AuthResponseServerPrivate: public QObject
{
    Q_OBJECT
public:
    AuthResponseServerPrivate(AuthResponseServer * parent);
    virtual ~AuthResponseServerPrivate();
    QMultiMap<QString, QString> parseQueryParams(QByteArray *sdata);

public Q_SLOTS:
    void onIncomingConnection();
    void onBytesReady();

public:
    AuthResponseServer * q_ptr;
    Q_DECLARE_PUBLIC(AuthResponseServer);

    QTcpSocket *socket;
    QString localFile;

private:
    bool handlingRedirect;
};

#endif /* AUTHRESPONSESERVER_P_HPP_ */

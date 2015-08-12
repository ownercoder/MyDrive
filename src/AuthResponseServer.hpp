/*
 * AuthResponseServer.hpp
 *
 *  Created on: 22 марта 2015 г.
 *      Author: Sergey
 */

#ifndef AUTHRESPONSESERVER_HPP_
#define AUTHRESPONSESERVER_HPP_

#include <QObject>
#include <QTcpServer>

class AuthResponseServerPrivate;
class AuthResponseServer: public QTcpServer
{
    Q_OBJECT
public:
    explicit AuthResponseServer(QObject * parent);
    virtual ~AuthResponseServer();
    void setSuccessHtmlFile(QString filePath);

Q_SIGNALS:
    void verificationReceived(QMultiMap<QString, QString>);

private:
    AuthResponseServerPrivate * const d_ptr;
    Q_DECLARE_PRIVATE(AuthResponseServer);
};

#endif /* AUTHRESPONSESERVER_HPP_ */

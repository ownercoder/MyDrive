/*
 * AuthResponseServer.cpp
 *
 *  Created on: 22 марта 2015 г.
 *      Author: Sergey
 */

#include <QTcpSocket>
#include <QStringList>
#include <QUrl>
#include <QFile>
#include <QDir>
#include <QFileInfoList>

#include "AuthResponseServer.hpp"
#include "AuthResponseServer_p.hpp"

// Private implementation
AuthResponseServerPrivate::AuthResponseServerPrivate(AuthResponseServer *parent):
    q_ptr(parent) {

}

AuthResponseServerPrivate::~AuthResponseServerPrivate() {

}

void AuthResponseServerPrivate::onIncomingConnection() {
    Q_Q(AuthResponseServer);
    qDebug() << "Incoming Connection";
    socket = q->nextPendingConnection();
    connect(socket, SIGNAL(readyRead()),
            this, SLOT(onBytesReady()), Qt::UniqueConnection);
}

void AuthResponseServerPrivate::onBytesReady() {
    Q_Q(AuthResponseServer);
    qDebug() << "Socket peer host address: " << socket->peerAddress();
    QByteArray reply;
    QByteArray content;

     QByteArray data = socket->readAll();
     qDebug()<< "Query Data: " << data;
     QMultiMap<QString, QString> queryParams = parseQueryParams(&data);
     if(queryParams.size() == 0 && !handlingRedirect) { //assume theres a hash and do the redirect hack
         handlingRedirect = true;
         content.append("<HTML><head><script type=\"text/javascript\">var str='http://'+window.location.host + '?' + window.location.hash.substring(1); window.location=str;</script></head><h1>Account authorized, go ahead back to the tumblr app and start your experience!</h1></HTML>");
     } else {
         handlingRedirect = false;
        QFile file("app/native/assets/" + localFile);
        QString fileData;
         if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
             qDebug() << "file worked";
             QTextStream in(&file);
             while (!in.atEnd()) {
                 fileData += in.readLine();
             }
             file.close();
         }
         if(fileData.isEmpty()) {
             content.append("<HTML><h1>Account linked, go ahead back to the app and check the status!</h1></HTML>");
         } else {
             content.append(fileData);
         }
     }

    reply.append("HTTP/1.0 200 OK \r\n");
    reply.append("Content-Type: text/html; charset=\"utf-8\"\r\n");
    reply.append(QString("Content-Length: %1\r\n").arg(content.size()));
    reply.append("\r\n");
    reply.append(content);
    socket->write(reply);

    if(!handlingRedirect) {
        socket->disconnectFromHost();
        q->close();
        emit q->verificationReceived(queryParams);
    }
}

QMultiMap<QString, QString> AuthResponseServerPrivate::parseQueryParams(QByteArray *data) {
    QString splitGetLine = QString(*data).split("\r\n").first();   // Retrieve the first line with query params.
    splitGetLine.remove("GET ");                                   // Clean the line from GET
    splitGetLine.remove("HTTP/1.1");                               // From HTTP
    splitGetLine.remove("\r\n");                                   // And from rest.
    splitGetLine.prepend("http://localhost");                      // Now, make it a URL

    QUrl getTokenUrl(splitGetLine);
    QList< QPair<QString, QString> > tokens = getTokenUrl.queryItems();  // Ask QUrl to do our work.

    QMultiMap<QString, QString> queryParams;
    QPair<QString, QString> tokenPair;
    foreach (tokenPair, tokens) {
        queryParams.insert(tokenPair.first.trimmed(), tokenPair.second.trimmed());
    }

    return queryParams;
}

// END Private implementation

AuthResponseServer::AuthResponseServer(QObject *parent) :
    QTcpServer(parent),
    d_ptr( new AuthResponseServerPrivate(this) ) {
    Q_D(AuthResponseServer);

    connect(this, SIGNAL(newConnection()),
            d, SLOT(onIncomingConnection()));
}

AuthResponseServer::~AuthResponseServer() {
    delete d_ptr;
}


void AuthResponseServer::setSuccessHtmlFile(QString filePath) {
    Q_D(AuthResponseServer);
    d->localFile = filePath;
}



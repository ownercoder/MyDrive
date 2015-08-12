/*
 * RequestManager.hpp
 *
 *  Created on: 25 апр. 2015 г.
 *      Author: Sergey
 */

#ifndef REQUESTMANAGER_HPP_
#define REQUESTMANAGER_HPP_

#include <QObject>
#include <QNetworkAccessManager>

#include "SessionManager.hpp"

class RequestManagerPrivate;
class RequestManager: public QNetworkAccessManager {
    Q_OBJECT
public:
    explicit RequestManager(QObject * parent = 0);
    enum RequestMethod {
        MethodGET,
        MethodPUT,
        MethodPOST
    };

protected:
    void setSessionManager(SessionManager *sessionManager);
    QNetworkReply* request(const QString &url, GoogleSession session = GoogleSession(), QMultiMap<QString, QString> requestData = QMultiMap<QString, QString>(), RequestMethod method = RequestManager::MethodGET);
private:
    SessionManager *sessM;

    Q_DECLARE_PRIVATE(RequestManager);
    RequestManagerPrivate * const d_ptr;
};

#endif /* REQUESTMANAGER_HPP_ */

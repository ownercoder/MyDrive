/*
 * ApiWrapper.hpp
 *
 *  Created on: 11 мая 2015 г.
 *      Author: Sergey
 */

#ifndef APIWRAPPER_HPP_
#define APIWRAPPER_HPP_

#include <QQueue>
#include <QNetworkReply>

#include "FileMetaData.hpp"
#include "SessionManager.hpp"
#include "TokenManager.hpp"
#include "RequestManager.hpp"

class ApiWrapper: public RequestManager {
    Q_OBJECT
public:
    ApiWrapper(QObject * parent = 0);
    virtual ~ApiWrapper();

    Q_INVOKABLE void addToQueue(const QVariantMap &data);
    Q_INVOKABLE void trash();
    Q_INVOKABLE void createDirectory(const QString &name, const QString &parent = "root");

    Q_INVOKABLE void abort();

Q_SIGNALS:
    void error(const QString &text);
    void requestFinished();
    void requestStarted();
    void folderCreated(const QVariantMap &data);
protected Q_SLOTS:
    void onFinished();
private:
    void processTrash();

    TokenManager tokenManager;
    SessionManager sessM;
    QNetworkReply *reply;
    QQueue<FileMetaData> _queue;
};

#endif /* APIWRAPPER_HPP_ */

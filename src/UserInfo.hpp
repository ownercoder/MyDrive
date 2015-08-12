/*
 * UserInfo.hpp
 *
 *  Created on: 09 мая 2015 г.
 *      Author: Sergey
 */

#ifndef USERINFO_HPP_
#define USERINFO_HPP_

#include <QQueue>
#include <QNetworkReply>

#include "UserInfoData.hpp"
#include "RequestManager.hpp"

class UserInfo: public RequestManager {
    Q_OBJECT
public:
    UserInfo(QObject * parent = 0);
    void getUserDataInfo(QList<GoogleSession> sessions);

Q_SIGNALS:
    void infoReceived(QList<UserInfoData> info);

protected Q_SLOTS:
    void onFinished();

protected:
    void getNext();

    SessionManager *sessM;
    GoogleSession currentRequestSession;
    QQueue<GoogleSession> queue;
    QList<UserInfoData> userInfos;
    QNetworkReply *reply;
};

#endif /* USERINFO_HPP_ */

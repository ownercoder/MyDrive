/*
 * AccountDataModel.hpp
 *
 *  Created on: 09 мая 2015 г.
 *      Author: Sergey
 */

#ifndef ACCOUNTDATAMODEL_HPP_
#define ACCOUNTDATAMODEL_HPP_

#include <bb/cascades/QListDataModel>

#include "SessionManager.hpp"
#include "TokenManager.hpp"
#include "UserInfo.hpp"

using namespace bb::cascades;

class AccountDataModel: public QVariantListDataModel {
    Q_OBJECT
public:
    AccountDataModel();

    Q_INVOKABLE void load();

Q_SIGNALS:
    void startLoad();
    void finishLoad();
protected Q_SLOTS:
    void onInfoReceived(QList<UserInfoData> info);
private:
    TokenManager *tokenManager;
    SessionManager *sessionManager;
    UserInfo *userInfo;
};

#endif /* ACCOUNTDATAMODEL_HPP_ */

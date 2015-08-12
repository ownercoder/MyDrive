/*
 * AccountDataModel.cpp
 *
 *  Created on: 09 мая 2015 г.
 *      Author: Sergey
 */

#include "AccountDataModel.hpp"

AccountDataModel::AccountDataModel() {
    tokenManager = new TokenManager();
    sessionManager = new SessionManager();
    sessionManager->setTokenManager(tokenManager);
    userInfo = new UserInfo();
    connect(userInfo, SIGNAL(infoReceived(QList<UserInfoData>)), this, SLOT(onInfoReceived(QList<UserInfoData>)));
}

void AccountDataModel::load() {
    clear();
    emit startLoad();
    userInfo->getUserDataInfo(sessionManager->getSessions());
}

void AccountDataModel::onInfoReceived(QList<UserInfoData> info) {
    QVariantList list;

    for(int i=0;i<info.length();i++) {
        list.append(info.at(i).toMap());
    }

    QVariantMap addAccount;
    addAccount.insert("addAccount", true);
    list.append(addAccount);

    append(list);
    emit finishLoad();
}

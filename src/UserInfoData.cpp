/*
 * UserInfoData.cpp
 *
 *  Created on: 09 мая 2015 г.
 *      Author: Sergey
 */

#include <QRegExp>

#include "UserInfoData.hpp"

UserInfoData::UserInfoData() {

}

UserInfoData::UserInfoData(const QVariantMap& data, const QString &accountId) {
    displayName = data.value("user").value<QVariantMap>().value("displayName").toString();
    picture = data.value("user").value<QVariantMap>().value("picture").value<QVariantMap>().value("url").toString();
    emailAddress = data.value("user").value<QVariantMap>().value("emailAddress").toString();

    picture480 = QString(picture).replace(QRegExp("\\/s[0-9]+\\/"), "/s480/");
    internalAccountId = accountId;
}

QVariantMap UserInfoData::toMap() const {
    QVariantMap data;
    data.insert("displayName", displayName);
    data.insert("picture", picture);
    data.insert("picture480", picture480);
    data.insert("emailAddress", emailAddress);
    data.insert("accountId", internalAccountId);
    return data;
}

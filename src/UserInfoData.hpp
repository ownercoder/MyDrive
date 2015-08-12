/*
 * UserInfoData.hpp
 *
 *  Created on: 09 мая 2015 г.
 *      Author: Sergey
 */

#ifndef USERINFODATA_HPP_
#define USERINFODATA_HPP_

class UserInfoData {
public:
    UserInfoData();
    UserInfoData(const QVariantMap &data, const QString &accountId = "");
    QVariantMap toMap() const;

    QString displayName;
    QString picture;
    QString picture480;
    QString emailAddress;
    QString internalAccountId;
};

Q_DECLARE_METATYPE(QList<UserInfoData>)

#endif /* USERINFODATA_HPP_ */

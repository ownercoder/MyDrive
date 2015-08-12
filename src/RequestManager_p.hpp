/*
 * RequestManager_p.hpp
 *
 *  Created on: 25 апр. 2015 г.
 *      Author: Sergey
 */

#ifndef REQUESTMANAGER_P_HPP_
#define REQUESTMANAGER_P_HPP_

#include <QUrl>
#include <QNetworkReply>

#include "RequestManager.hpp"

class RequestManagerPrivate : public QObject {
    Q_OBJECT
public:
    RequestManagerPrivate(RequestManager * parent);
    virtual ~RequestManagerPrivate();

    RequestManager* const q_ptr;
    Q_DECLARE_PUBLIC(RequestManager);
};



#endif /* REQUESTMANAGER_P_HPP_ */

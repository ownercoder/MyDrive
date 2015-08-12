/*
 * SessionWrapper.hpp
 *
 *  Created on: 09 мая 2015 г.
 *      Author: Sergey
 */

#ifndef SESSIONWRAPPER_HPP_
#define SESSIONWRAPPER_HPP_

#include <QObject>

#include "SessionManager.hpp"

class SessionWrapper: public QObject {
    Q_OBJECT
public:
    SessionWrapper(QObject * parent = 0);
    virtual ~SessionWrapper();

    Q_INVOKABLE QString getLoginUrl();
    Q_INVOKABLE void changeUserAccount(const QString &id);
    Q_INVOKABLE void exitCurrentSession();
Q_SIGNALS:
    void userSessionCreated();
protected Q_SLOTS:
    void onSessionCreated(GoogleSession session);
protected:
    SessionManager *sessManager;
};

#endif /* SESSIONWRAPPER_HPP_ */

/*
 * Copyright (c) 2011-2014 BlackBerry Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ApplicationUI_HPP_
#define ApplicationUI_HPP_

#include <QObject>
#include <bb/cascades/AbstractPane>
#include "GoogleSession.hpp"
#include "SessionManager.hpp"
#include "FilesDataModel.hpp"
#include "ApplicationSettings.hpp"

namespace bb
{
    namespace cascades
    {
        class LocaleHandler;
    }
}

class QTranslator;

/*!
 * @brief Application UI object
 *
 * Use this object to create and init app UI, to create context objects, to register the new meta types etc.
 */
class ApplicationUI : public QObject
{
    Q_OBJECT
public:
    ApplicationUI(QObject * parent = 0);
    virtual ~ApplicationUI() {}

    Q_INVOKABLE QString getLoginUrl();
    Q_INVOKABLE void changeAccount(const QString &id);
protected:
    void showGoogleMain();
    void showLogin();
    void showWelcome();
    SessionManager * sessionManager;
    GoogleSession currentSession;
private slots:
    void onSystemLanguageChanged();
    void onSessionCreated(GoogleSession session);
private:
    ApplicationSettings *settings;
    QTranslator* m_pTranslator;
    bb::cascades::LocaleHandler* m_pLocaleHandler;
    // Root pane of the application scene.
    bb::cascades::AbstractPane* mRoot;
};

#endif /* ApplicationUI_HPP_ */

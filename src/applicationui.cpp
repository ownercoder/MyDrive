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

#include "applicationui.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/LocaleHandler>
#include "support/qmlbeam.h"
#include "SessionManager.hpp"
#include "TokenManager.hpp"
#include "GoogleFiles.hpp"
#include "WebImageView.hpp"
#include "ApplicationSettings.hpp"
#include "GoogleDriveDownloader.hpp"
#include "GoogleFileInvoker.hpp"
#include "SearchDataModel.hpp"
#include "AccountDataModel.hpp"
#include "SessionWrapper.hpp"
#include "ApiWrapper.hpp"
#include "GoogleDriveUploader.hpp"

using namespace bb::cascades;

ApplicationUI::ApplicationUI(QObject * parent) :
        QObject(parent) {
    // prepare the localization
    m_pTranslator = new QTranslator(this);
    m_pLocaleHandler = new LocaleHandler(this);

    bool res = QObject::connect(m_pLocaleHandler, SIGNAL(systemLanguageChanged()), this, SLOT(onSystemLanguageChanged()));
    // This is only available in Debug builds
    Q_ASSERT(res);
    // Since the variable is not used in the app, this is added to avoid a
    // compiler warning
    Q_UNUSED(res);

    // initial load
    onSystemLanguageChanged();

    settings = new ApplicationSettings();

    qRegisterMetaType< QList<FileMetaData> >("QList<FileMetaData>");

    qmlRegisterType<FilesDataModel>("com.slytech.filesdatamodel", 1, 0, "FilesDataModel");
    qmlRegisterType<GoogleFiles>("com.slytech.googlefiles", 1, 0, "GoogleFiles");
    qmlRegisterType<GoogleDriveDownloader>("com.slytech.googledownloader",1, 0, "GoogleDriveDownloader");
    qmlRegisterType<GoogleFileInvoker>("com.slytech.googlefileinvoker",1, 0, "GoogleFileInvoker");
    qmlRegisterType<SearchDataModel>("com.slytech.searchdatamodel",1, 0, "SearchDataModel");
    qmlRegisterType<AccountDataModel>("com.slytech.accountdatamodel",1, 0, "AccountDataModel");
    qmlRegisterType<SessionWrapper>("com.slytech.sessionwrapper",1, 0, "SessionWrapper");
    qmlRegisterType<ApiWrapper>("com.slytech.apiwrapper",1, 0, "ApiWrapper");
    qmlRegisterType<GoogleDriveUploader>("com.slytech.googleuploader", 1, 0, "GoogleDriveUploader");
    qmlRegisterType<WebImageView>("my.library",1, 0, "WebImageView");

    TokenManager * tokenManager = new TokenManager();
    connect(tokenManager, SIGNAL(sessionCreated(GoogleSession)), this, SLOT(onSessionCreated(GoogleSession)));

    sessionManager = new SessionManager();
    sessionManager->setTokenManager(tokenManager);

    currentSession = sessionManager->getCurrentSession();
    if (currentSession.token.length() == 0) {
        showLogin();
    } else {
        showGoogleMain();
    }
    new QmlBeam(this);
}

QString ApplicationUI::getLoginUrl() {
    return sessionManager->getAuthenticationUrl(SessionManager::Drive | SessionManager::Profile | SessionManager::Email).toString();
}

void ApplicationUI::changeAccount(const QString &id) {
    QList<GoogleSession> sessionList = sessionManager->getSessions();
    for(int i=0;i<sessionList.length();i++) {
        if (sessionList.at(i).uniqId == id) {
            sessionManager->setCurrentSession(sessionList.at(i));
            return;
        }
    }

    qDebug() << "Cannot find session id:" << id;
    return;
}

void ApplicationUI::onSessionCreated(GoogleSession session) {
    qDebug() << "Session created: " << session.token;
    sessionManager->storeSession(session);
    sessionManager->setCurrentSession(session);
    showGoogleMain();
}

void ApplicationUI::onSystemLanguageChanged() {
    QCoreApplication::instance()->removeTranslator(m_pTranslator);
    // Initiate, load and install the application translation files.
    QString locale_string = QLocale().name();
    QString file_name = QString("MyDrive_%1").arg(locale_string);
    if (m_pTranslator->load(file_name, "app/native/qm")) {
        QCoreApplication::instance()->installTranslator(m_pTranslator);
    }
}

void ApplicationUI::showGoogleMain() {
//    // Get the context of the first scene root to keep the contextProperties.
//    QDeclarativeContext* context = QDeclarativeEngine::contextForObject(mRoot);
//
//    if (context) {
//        // Clear the QML cache.
//        QDeclarativeEngine* appEngine = context->engine();
//        appEngine->clearComponentCache();
//    }

    QmlDocument *qml = QmlDocument::create("asset:///GoogleMain.qml");

    qml->setContextProperty("app", this);
    qml->setContextProperty("settings_", settings);

    // Create root object for the UI
    AbstractPane *root = qml->createRootObject<AbstractPane>();

    Application::instance()->setScene(root);
}

void ApplicationUI::showLogin() {
    //delete Application::instance()->scene();

    QmlDocument *qml = QmlDocument::create("asset:///LoginView.qml").parent(this);

    qml->setContextProperty("app", this);

    // Create root object for the UI
    AbstractPane *root = qml->createRootObject<AbstractPane>();

    Application::instance()->setScene(root);
}

void ApplicationUI::showWelcome() {
    //delete Application::instance()->scene();

    QmlDocument *qml = QmlDocument::create("asset:///Welcome.qml").parent(this);

    qml->setContextProperty("app", this);

    // Create root object for the UI
    AbstractPane *root = qml->createRootObject<AbstractPane>();

    Application::instance()->setScene(root);
}

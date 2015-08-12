/*
 * GoogleFileInvoker.cpp
 *
 *  Created on: 03 мая 2015 г.
 *      Author: Sergey
 */
#include <QFileInfo>
#include <QDebug>
#include <bb/system/InvokeManager>
#include <bb/system/InvokeAction>
#include <bb/system/InvokeTarget>
#include <bb/system/InvokeRequest>
#include <bb/system/InvokeTargetReply>
#include <bb/system/InvokeReplyError>
#include <bb/system/InvokeQueryTargetsRequest>
#include <bb/system/InvokeQueryTargetsReply>

#include "GoogleFileInvoker.hpp"

using namespace bb::system;

GoogleFileInvoker::GoogleFileInvoker(QObject * parent) : QObject(parent) {

}

void GoogleFileInvoker::setFilename(const QString& filename) {
    _filename = filename;
}

void GoogleFileInvoker::open(const QString& mimeType) {
    _mimeType = mimeType;

    InvokeManager invokeManager;
    InvokeQueryTargetsRequest request;

    // Find the targets that support bb.action.OPEN
    request.setAction("bb.action.VIEW");

    // Find the targets that can handle image/png data
    request.setMimeType(mimeType);

    // Find the targets that are sent as a file from this path
    request.setUri(QUrl("file://" + _filename));
    InvokeQueryTargetsReply* results = invokeManager.queryTargets(request);

    if (results) {
        // Remember to take ownership of the results
        results->setParent(this);

        // Listen for the results
        connect(results, SIGNAL(finished()), this, SLOT(onQueryResponse()));
    } else {
        emit error(tr("Unsupported file"));
    }
}

void GoogleFileInvoker::onQueryResponse() {
    InvokeQueryTargetsReply* _queryResults = dynamic_cast<InvokeQueryTargetsReply*>(sender());

    switch(_queryResults->error()) {
        case InvokeReplyError::BadRequest:
            emit error(tr("Bad request"));
            break;
        case InvokeReplyError::Internal:
            emit error(tr("Internal error"));
            break;
        default:
            // If the error is resolved, proceed
            QList<InvokeAction> targetsByAction = _queryResults->actions();
            QString actionName, targetId;
            bool breakFor = false;

            for (int i = 0; i < targetsByAction.size(); i++) {
                actionName = targetsByAction[i].name();
                QList<InvokeTarget> targets = targetsByAction[i].targets();
                for (int j = 0; j < targets.size(); j++) {
                    targetId = targets[j].name();

                    if (targetId.contains(".card.")) {
                        breakFor = true;
                        break;
                    }
                }

                if (breakFor) break;
            }

            if (!actionName.length() || !targetId.length()) {
                emit error(tr("Unsupported file"));
                return;
            }

            InvokeManager invokeManager;
            InvokeRequest request;
            // Set the target app
            request.setTarget(targetId);
            // Set the action that the target app should execute
            request.setAction(actionName);
            // Set the MIME type of the data
            request.setMimeType(_mimeType);
            // Specify the location of the data
            request.setUri(QUrl("file://" + _filename));
            InvokeTargetReply *reply = invokeManager.invoke(request);
            connect(reply, SIGNAL(finished()), this, SLOT(onInvokeResult()));
    }

    // Free the resources that were allocated for the query
    delete _queryResults;
}

void GoogleFileInvoker::onInvokeResult() {
    InvokeTargetReply *_invokeTargetReply = qobject_cast<InvokeTargetReply*>(sender());
    // Check for errors
    switch(_invokeTargetReply->error()) {
        // Invocation could not find the target;
        // did we use the right target ID?
        case InvokeReplyError::NoTarget: {
            qDebug() << "invokeFinished(): Error: no target";
            break;
        }

        // There was a problem with the invocation request;
        // did we set all of the values correctly?
        case InvokeReplyError::BadRequest: {
            qDebug() << "invokeFinished(): Error: bad request";
            break;
        }

        // Something went completely
        // wrong inside the invocation request,
        // so find an alternate route
        case InvokeReplyError::Internal: {
            qDebug() << "invokeFinished(): Error: internal";
            break;
        }

        // Message received if the invocation request is successful
        default:
            qDebug() << "invokeFinished(): Invoke Succeeded";
            break;
    }

    // Free the resources that were allocated for the reply
    delete _invokeTargetReply;
}

QString GoogleFileInvoker::filename() const {
    return _filename;
}

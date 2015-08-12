/*
 * GoogleDriveUploader.hpp
 *
 *  Created on: 13 июня 2015 г.
 *      Author: Sergey
 */

#ifndef GOOGLEDRIVEUPLOADER_HPP_
#define GOOGLEDRIVEUPLOADER_HPP_

#include <QQueue>
#include <QNetworkReply>
#include "RequestManager.hpp"

class GoogleDriveUploader: public RequestManager
{
    Q_OBJECT
public:
    GoogleDriveUploader(QObject * parent = 0);

    Q_INVOKABLE void uploadTo(const QString &path);
    Q_INVOKABLE void addToQueue(const QString& path);
    Q_INVOKABLE void abort();

Q_SIGNALS:
    void uploadStarted();
    void uploadCompleted();
    void uploadProgress(qint64 bytesSent, qint64 bytesTotal, const QString &fileName);
    void error(const QString &errorText);
protected Q_SLOTS:
    void onUploadProgress(qint64 bytesSent, qint64 bytesTotal);
    void onFinished();
private:
    QString currentUploadPath;
    QString currentFilePath;
    QString currentFileName;
    qint64 totalBytesForSend;
    qint64 totalBytesSent;
    QQueue<QString> _queue;
    QNetworkReply *reply;
};

#endif /* GOOGLEDRIVEUPLOADER_HPP_ */

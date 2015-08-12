/*
 * GoogleFiles.hpp
 *
 *  Created on: 25 апр. 2015 г.
 *      Author: Sergey
 */

#ifndef GOOGLEFILES_HPP_
#define GOOGLEFILES_HPP_

#include <QNetworkReply>

#include "RequestManager.hpp"
#include "FileMetaData.hpp"

class GoogleFiles: public RequestManager {
    Q_OBJECT
    Q_ENUMS(GoogleFolder)
public:
    GoogleFiles(SessionManager *sessionManager = 0, QObject * parent = 0);

    enum GoogleFolder {
        NoFolder        = 0,
        MyFolder        = 1,
        SharedWithMe    = 2,
        Starred         = 3,
        Trashed         = 4
    };
    Q_FLAGS(GoogleFolder)

    void list(const QString &parent = "root", GoogleFolder folderType = GoogleFiles::NoFolder, const QString &pageToken = "");
    void search(const QString &filename, const QString& pageToken = "", int maxItems = 20);
signals:
    void listCompleted(QList<FileMetaData> const files);
    void searchCompleted(QList<FileMetaData> const files, const QString &nextPageToken);
protected Q_SLOTS:
    void requestListFinished();
    void requestSearchFinished();
protected:
    SessionManager *sessM;
    QNetworkReply *reply;
    QList<FileMetaData> files;

    int searchMaxItems;
    QString searchNextPageToken;

};

#endif /* GOOGLEFILES_HPP_ */

/*
 * FilesDataModel.cpp
 *
 *  Created on: 23 апр. 2015 г.
 *      Author: Sergey
 */

#include <src/FilesDataModel.hpp>
#include <bb/cascades/QListDataModel>
#include <QDebug>

#include "FileMetaDataSort.hpp"
#include "GoogleFiles.hpp"

FilesDataModel::FilesDataModel(QObject * parent) {
    Q_UNUSED(parent);
    tokenManager = new TokenManager();
    sessionManager = new SessionManager();
    sessionManager->setTokenManager(tokenManager);
    parentId = "root";
}

QString FilesDataModel::parent() {
    return parentId;
}

void FilesDataModel::load() {
    emit startLoad();
    clear();
    GoogleFiles * googleFiles = new GoogleFiles(sessionManager);
    connect(googleFiles, SIGNAL(listCompleted(QList<FileMetaData>)), this, SLOT(onFilesReceived(QList<FileMetaData>)));
    googleFiles->list(parentId, googleFolderType);
}

void FilesDataModel::onFilesReceived(QList<FileMetaData> const list) {
    GoogleFiles *googleFiles = qobject_cast<GoogleFiles*>(sender());
    googleFiles->deleteLater();

    QList<FileMetaData> folders;
    QList<FileMetaData> files;

    for(int i = 0;i<list.length();i++) {
        if (list.at(i).isFolder) {
            folders << list.at(i);
        } else {
            files << list.at(i);
        }
    }

    qSort(folders.begin(), folders.end(), lessThanFileMetaData);
    qSort(files.begin(), files.end(), lessThanFileMetaData);

    appendFileMetaDataList(folders);
    appendFileMetaDataList(files);

    emit finishLoad();
}

void FilesDataModel::setParent(const QString& id) {
    parentId = id;
    load();
}

int FilesDataModel::folderType() {
    return googleFolderType;
}

void FilesDataModel::setFolderType(int type) {
    googleFolderType = static_cast<GoogleFiles::GoogleFolder>(type);
}

void FilesDataModel::appendFileMetaDataList(const QList<FileMetaData>& data) {
    for(int i=0;i<data.length();i++) {
        append(data.at(i).toMap());
    }
}

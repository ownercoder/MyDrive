/*
 * FilesDataModel.hpp
 *
 *  Created on: 23 апр. 2015 г.
 *      Author: Sergey
 */

#ifndef FILESDATAMODEL_HPP_
#define FILESDATAMODEL_HPP_

#include <bb/cascades/QListDataModel>

#include "SessionManager.hpp"
#include "FileMetaData.hpp"
#include "GoogleFiles.hpp"
#include "TokenManager.hpp"

class FilesDataModel: public bb::cascades::QVariantListDataModel {
    Q_OBJECT
    Q_PROPERTY(QString parentId READ parent WRITE setParent NOTIFY parentFolderChanged)
    Q_PROPERTY(int folderType READ folderType WRITE setFolderType)
public:
    FilesDataModel(QObject * parent = 0);

    Q_INVOKABLE void load();

    QString parent();
    void setParent(const QString &id);
    int folderType();
    void setFolderType(int type);
signals:
    void parentFolderChanged(const QString &folder);
    void startLoad();
    void finishLoad();

protected Q_SLOTS:
    void onFilesReceived(QList<FileMetaData> const list);
private:
    void appendFileMetaDataList(const QList<FileMetaData>& data);
    TokenManager *tokenManager;
    SessionManager * sessionManager;
    QString parentId;

    GoogleFiles::GoogleFolder googleFolderType;
    QList<FileMetaData> files;
};

Q_DECLARE_METATYPE( FilesDataModel* )


#endif /* FILESDATAMODEL_HPP_ */

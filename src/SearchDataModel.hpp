/*
 * SearchDataModel.hpp
 *
 *  Created on: 05 мая 2015 г.
 *      Author: Sergey
 */

#ifndef SEARCHDATAMODEL_HPP_
#define SEARCHDATAMODEL_HPP_

#include <bb/cascades/QListDataModel>
#include "GoogleFiles.hpp"
#include "FileMetaData.hpp"

using namespace bb::cascades;

class SearchDataModel: public QVariantListDataModel {
    Q_OBJECT
public:
    SearchDataModel();

    Q_INVOKABLE void searchFile(const QString& filename);
    Q_INVOKABLE void fetchMore();
    Q_INVOKABLE bool canFetchMore();
    Q_INVOKABLE QString itemType(const QVariantList &indexPath);

public Q_SLOTS:
    void onSearchCompleted(QList<FileMetaData> const files, const QString &nextPageToken);

private:
    void addLoadingItem();
    void addFetchMoreItem();
    void removeLoadingItem();
    void removeFetchMoreItem();

    GoogleFiles *googleFiles;
    SessionManager *sessionManager;
    QString nextSearchPageToken;
    QString searchText;
    int loadingItemIndexPath;
    int fetchMoreItemIndexPath;
};

#endif /* SEARCHDATAMODEL_HPP_ */

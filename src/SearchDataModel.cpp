/*
 * SearchDataModel.cpp
 *
 *  Created on: 05 мая 2015 г.
 *      Author: Sergey
 */
#include <QDebug>
#include "SearchDataModel.hpp"

SearchDataModel::SearchDataModel() {
    sessionManager = new SessionManager();
    googleFiles = new GoogleFiles(sessionManager);
    connect(googleFiles, SIGNAL(searchCompleted(QList<FileMetaData>, const QString&)), this, SLOT(onSearchCompleted(QList<FileMetaData>, const QString&)));
}

void SearchDataModel::searchFile(const QString& filename) {
    nextSearchPageToken = "";
    searchText = filename;
    clear();
    if (filename.length()) {
        addLoadingItem();
        googleFiles->search(searchText);
    }
}

QString SearchDataModel::itemType(const QVariantList &indexPath) {
    if (value(indexPath.at(0).toInt()).toMap().value("loadingItem").toBool() == true) {
        return "loadingItem";
    } else if (value(indexPath.at(0).toInt()).toMap().value("fetchMoreItem").toBool() == true) {
        return "fetchMoreItem";
    }
    return "listItem";
}

void SearchDataModel::onSearchCompleted(QList<FileMetaData> const files,
        const QString& nextPageToken) {
    removeLoadingItem();
    nextSearchPageToken = nextPageToken;
    QVariantList dataToAppend;
    for(int i = 0;i < files.length();i++) {
        dataToAppend.append(files.at(i).toMap());
    }
    append(dataToAppend);
    if (canFetchMore()) {
        addFetchMoreItem();
    } else {
        removeFetchMoreItem();
    }
}

bool SearchDataModel::canFetchMore() {
    qDebug() << nextSearchPageToken.length();
    return nextSearchPageToken.length();
}

void SearchDataModel::fetchMore() {
    removeFetchMoreItem();
    addLoadingItem();
    googleFiles->search(searchText, nextSearchPageToken);
}

void SearchDataModel::addLoadingItem() {
    QVariantMap map;
    map["loadingItem"] = true;
    append(map);
    loadingItemIndexPath = size()-1;
}

void SearchDataModel::addFetchMoreItem() {
    QVariantMap map;
    map["fetchMoreItem"] = true;
    append(map);
    fetchMoreItemIndexPath = size()-1;
}

void SearchDataModel::removeFetchMoreItem() {
    removeAt(fetchMoreItemIndexPath);
}

void SearchDataModel::removeLoadingItem() {
    removeAt(loadingItemIndexPath);
}

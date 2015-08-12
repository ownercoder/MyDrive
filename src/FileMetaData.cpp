/*
 * FileMetaData.cpp
 *
 *  Created on: 25 апр. 2015 г.
 *      Author: Sergey
 */

#include <QDebug>
#include "FileMetaData.hpp"

FileMetaData::FileMetaData() {
    _isEmpty = true;
}

FileMetaData::FileMetaData(const QVariantMap &data) {
    id = data.value("id").toString();
    title = data.value("title").toString();
    mimeType = data.value("mimeType").toString();
    thumbnailLink = data.value("thumbnailLink").toString();
    isFolder = mimeType == "application/vnd.google-apps.folder" ? true : false;
    mimeTypeImage = getMimeImageUrl(mimeType);
    folderColorRgb = data.value("folderColorRgb").toString();
    downloadUrl = data.value("downloadUrl").toString();
    parentIds = data.value("parents").value<QVariantMap>();
    exportLinks = data.value("exportLinks").value<QVariantMap>();
    fileSize = data.value("fileSize").toUInt();
    shared = data.value("shared").toBool();
    writersCanShare = data.value("writersCanShare").toBool();
    labels = data.value("labels").value<QVariantMap>();
    if (data.value("modifiedDate").type() == QVariant::DateTime) {
        modifiedDate = data.value("modifiedDate").toDateTime();
    } else {
        modifiedDate = QDateTime::fromString(data.value("modifiedDate").toString(), GOOGLE_DATETIME_FORMAT);
    }

    _isEmpty = data.empty();
}

bool FileMetaData::isEmpty() {
    return _isEmpty;
}

QString FileMetaData::getMimeImageUrl(const QString &mimeType) {
    if (mimeType == "application/vnd.google-apps.folder") {
        return "asset:///images/folder.png";
    } else if (mimeType == "application/vnd.google-apps.audio") {
        return "asset:///images/files/audio.png";
    } else if (mimeType == "application/vnd.google-apps.spreadsheet") {
        return "asset:///images/files/excel.png";
    } else if (mimeType == "application/zip") {
        return "asset:///images/files/archive.png";
    } else if (mimeType == "application/vnd.google-apps.drawing") {
        return "asset:///images/files/image.png";
    } else if (mimeType == "application/vnd.google-apps.video") {
        return "asset:///images/files/video.png";
    } else if (mimeType == "application/vnd.google-apps.document") {
        return "asset:///images/files/text.png";
    } else {
        return "asset:///images/files/unknown.png";
    }
}

FileMetaData::~FileMetaData() {

}

QVariantMap FileMetaData::toMap() const {
    QVariantMap data;
    data.insert("id", id);
    data.insert("title", title);
    data.insert("mimeType", mimeType);
    data.insert("parentIds", parentIds);
    data.insert("thumbnailLink", thumbnailLink);
    data.insert("folderColorRgb", folderColorRgb);
    data.insert("mimeTypeImage", mimeTypeImage);
    data.insert("isFolder", isFolder);
    data.insert("downloadUrl", downloadUrl);
    data.insert("fileSize", fileSize);
    data.insert("exportLinks", exportLinks);
    data.insert("shared", shared);
    data.insert("labels", labels);
    data.insert("writersCanShare", writersCanShare);
    data.insert("modifiedDate", modifiedDate);

    return data;
}

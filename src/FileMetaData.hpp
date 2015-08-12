/*
 * FileMetaData.hpp
 *
 *  Created on: 25 апр. 2015 г.
 *      Author: Sergey
 */

#ifndef FILEMETADATA_HPP_
#define FILEMETADATA_HPP_

#define GOOGLE_DATETIME_FORMAT "yyyy-MM-ddTHH:mm:ss.zzzZ"

#include <QDateTime>

class FileMetaData {
public:
    FileMetaData(const QVariantMap &data);
    FileMetaData();
    bool isEmpty();
    virtual ~FileMetaData();
    QVariantMap toMap() const;

    QString id;
    QString title;
    QString mimeType;
    QVariantMap parentIds;
    QString thumbnailLink;
    QString folderColorRgb;
    QString mimeTypeImage;
    QString downloadUrl;
    QVariantMap exportLinks;
    quint64 fileSize;
    QDateTime modifiedDate;
    bool isFolder;
    bool shared;
    bool writersCanShare;
    QVariantMap labels;
protected:
    bool _isEmpty;
    QString getMimeImageUrl(const QString &mimeType);
};

typedef QList<FileMetaData> FileMetaDataList;

Q_DECLARE_METATYPE(QList<FileMetaData>)

#endif /* FILEMETADATA_HPP_ */

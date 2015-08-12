/*
 * GoogleFileInvoker.hpp
 *
 *  Created on: 03 мая 2015 г.
 *      Author: Sergey
 */

#ifndef GOOGLEFILEINVOKER_HPP_
#define GOOGLEFILEINVOKER_HPP_

class GoogleFileInvoker: public QObject {
    Q_OBJECT
    Q_PROPERTY(QString filename READ filename WRITE setFilename NOTIFY filenameChanged)
public:
    GoogleFileInvoker(QObject * parent = 0);

    void setFilename(const QString& filename);
    QString filename() const;

signals:
    void filenameChanged(const QString& filename);
    void error(const QString& error);

public Q_SLOTS:
    void open(const QString& mimeType);

protected Q_SLOTS:
    void onInvokeResult();
    void onQueryResponse();

protected:
    bool invokeImagePreview(const QString& mimeType);
    bool invokeDocsPreview(const QString& mimeType);
private:
    QString _filename;
    QString _mimeType;
};

#endif /* GOOGLEFILEINVOKER_HPP_ */

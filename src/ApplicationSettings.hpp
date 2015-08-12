/*
 * ApplicationSettings.hpp
 *
 *  Created on: 02 мая 2015 г.
 *      Author: Sergey
 */

#ifndef APPLICATIONSETTINGS_HPP_
#define APPLICATIONSETTINGS_HPP_

#include <QSettings>

class ApplicationSettings: public QSettings {
    Q_OBJECT
public:
    ApplicationSettings();
    Q_INVOKABLE void storeDirectoryView(const QString &directoryId, bool grid);
    Q_INVOKABLE bool directoryView(const QString &directoryId);
    Q_INVOKABLE void rememberExportFormat(const QString &sourceMimeType, const QString &destMimeType);
    Q_INVOKABLE QString getExportFormat(const QString &sourceMimeType);
    Q_INVOKABLE void forgetExportFormat(const QString &sourceMimeType);
};

#endif /* APPLICATIONSETTINGS_HPP_ */

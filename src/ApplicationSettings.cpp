/*
 * ApplicationSettings.cpp
 *
 *  Created on: 02 мая 2015 г.
 *      Author: Sergey
 */

#include <QDebug>
#include "ApplicationSettings.hpp"

ApplicationSettings::ApplicationSettings() {
    // TODO Auto-generated constructor stub

}

void ApplicationSettings::storeDirectoryView(const QString& directoryId, bool grid)
{
    qDebug() << "Set value for dir: " << directoryId << grid;
    setValue("dirPrefs/" + directoryId, grid);
}

bool ApplicationSettings::directoryView(const QString& directoryId)
{
    qDebug() << "Get value for dir: " << directoryId << value("dirPrefs/" + directoryId).toBool();
    return value("dirPrefs/" + directoryId).toBool();
}

void ApplicationSettings::rememberExportFormat(const QString& sourceMimeType,
        const QString& destMimeType)
{
    setValue("export/" + sourceMimeType, destMimeType);
}

QString ApplicationSettings::getExportFormat(const QString &sourceMimeType)
{
    return value("export/" + sourceMimeType).toString();
}

void ApplicationSettings::forgetExportFormat(const QString& sourceMimeType)
{
    remove("export/" + sourceMimeType);
}


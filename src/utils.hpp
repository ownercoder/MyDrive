/*
 * utils.hpp
 *
 *  Created on: 22 марта 2015 г.
 *      Author: Sergey
 */

#ifndef UTILS_HPP_
#define UTILS_HPP_

#include <QStringList>
#include <QString>
#include <QMap>

#include "FileMetaData.hpp"

template<int N>
Q_INLINE_TEMPLATE QStringList flagsToStrList(int i, const char *(&strings)[N]) {
    QStringList list;
    for (int pos = 0; pos < N; pos++) {
        int flag = 1 << pos;
        if ((flag) & i)
            list.append(strings[pos]);
    }
    return list;
}

#endif /* UTILS_HPP_ */

/*
 * FileMetaDataSort.hpp
 *
 *  Created on: 02 мая 2015 г.
 *      Author: Sergey
 */

#ifndef FILEMETADATASORT_HPP_
#define FILEMETADATASORT_HPP_


bool lessThanFileMetaData(const FileMetaData &a, const FileMetaData &b) {
    return a.title.toLower() < b.title.toLower();
}


#endif /* FILEMETADATASORT_HPP_ */

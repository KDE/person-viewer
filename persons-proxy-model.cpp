/*
    <one line to give the library's name and an idea of what it does.>
    Copyright (C) 2012  Martin Klapetek <martin.klapetek@gmail.com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/


#include "persons-proxy-model.h"
#include <KDebug>

PersonsProxyModel::PersonsProxyModel(QObject *parent)
    : KCategorizedSortFilterProxyModel(parent)
{

}

PersonsProxyModel::~PersonsProxyModel()
{

}

QVariant PersonsProxyModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    if (role == KCategorizedSortFilterProxyModel::CategoryDisplayRole) {
        return index.data(Qt::DisplayRole).toString().left(1);
    } else if (role == KCategorizedSortFilterProxyModel::CategorySortRole) {
//         kDebug() <<
        return index.data(Qt::DisplayRole).toString().left(1);
    }

    return QSortFilterProxyModel::data(index, role);
}

// int PersonsProxyModel::compareCategories(const QModelIndex& left, const QModelIndex& right) const
// {
//     return KCategorizedSortFilterProxyModel::compareCategories(left, right);
// }
//
// bool PersonsProxyModel::subSortLessThan(const QModelIndex &left, const QModelIndex &right) const
// {
//     return QString::localeAwareCompare(left.data(Qt::DisplayRole).toString(), right.data(Qt::DisplayRole).toString()) < 0;
// }


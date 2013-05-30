/*
    Copyright (C) 2012  Martin Klapetek <mklapetek@kde.org>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/


#ifndef PERSONS_PROXY_MODEL_H
#define PERSONS_PROXY_MODEL_H

#include <KCategorizedSortFilterProxyModel>

class PersonsProxyModel : public KCategorizedSortFilterProxyModel
{
    Q_OBJECT
public:
    PersonsProxyModel(QObject *parent = 0);
    ~PersonsProxyModel();

    virtual bool lessThan(const QModelIndex& left, const QModelIndex& right) const;
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
//     bool subSortLessThan(const QModelIndex &left, const QModelIndex &right) const;
//     int compareCategories(const QModelIndex &left, const QModelIndex &right) const;
};

#endif // PERSONS_PROXY_MODEL_H

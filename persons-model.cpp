/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2011  Martin Klapetek <email>

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

#include <Nepomuk/Resource>
#include <Nepomuk/ResourceManager>
#include <Nepomuk/Vocabulary/NCO>
#include <Nepomuk/Vocabulary/PIMO>
#include <Nepomuk/Variant>
#include <Nepomuk/Thing>

#include <nepomuk/simpleresource.h>
#include <nepomuk/simpleresourcegraph.h>

#include <Soprano/QueryResultIterator>
#include <Soprano/Node>
#include <Soprano/Model>

#include "persons-model.h"
#include <KDebug>

PersonsModel::PersonsModel()
    : m_resList(0)
{
    m_resList = new QList<Nepomuk::Resource>();

    QString query;
    query = "select ?r where { ?r a pimo:Person . }";

    Soprano::QueryResultIterator it = Nepomuk::ResourceManager::instance()->mainModel()->executeQuery(query,
                                                                                                      Soprano::Query::QueryLanguageSparql);

    while(it.next()) {
        kDebug() << Nepomuk::Resource(it["r"].uri()).allProperties();
        m_resList->append(Nepomuk::Resource(it["r"].uri()));
    }
}

PersonsModel::~PersonsModel()
{

}


QVariant PersonsModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    switch(role) {
        case Qt::DisplayRole:
            return m_resList->at(index.row()).property(Nepomuk::Vocabulary::PIMO::hasTag()).toResource().property(Nepomuk::Vocabulary::PIMO::tagLabel()).variant();
//         case PersonsModel::ItemRole:
            //return QVariant::fromValue<Nepomuk::Resource>(m_resList->at(index.row()));
        case PersonsModel::UriRole:
            return m_resList->at(index.row()).uri();
        default:
            return QVariant();
    }
}

int PersonsModel::rowCount(const QModelIndex& parent) const
{
    return m_resList->size();
}


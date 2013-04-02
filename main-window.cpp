/*
    Copyright (C) 2011  Martin Klapetek <mklapetek@kde.org>

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

#include "main-window.h"
#include "persons-delegate.h"
#include "persons-list-view.h"
#include "persons-proxy-model.h"

#include <KCategoryDrawer>
#include <KDebug>

#include <kpeople/persons-model.h>
#include <kpeople/persondata.h>

MainWindow::MainWindow(QWidget *parent)
    : KMainWindow(parent),
    m_personsModel(0),
    m_personsProxyModel(0),
    m_personsDelegate(0)
{
    setupUi(this);

    m_personsModel = new PersonsModel(this);
    connect(m_personsModel, SIGNAL(peopleAdded()),
            this, SLOT(onPersonModelReady()));

    connect(m_personsView, SIGNAL(clicked(QModelIndex)),
            this, SLOT(showContactDetails(QModelIndex)));

    connect(m_personsView, SIGNAL(selectedContacts(QItemSelection,QItemSelection)),
            this, SLOT(onSelectedContactsChanged(QItemSelection,QItemSelection)));

    m_personsView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_personsView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

MainWindow::~MainWindow()
{
}

void MainWindow::onPersonModelReady()
{
    m_personsProxyModel = new PersonsProxyModel(this);
    m_personsProxyModel->setSourceModel(m_personsModel);
    m_personsProxyModel->setDynamicSortFilter(true);
    m_personsProxyModel->setCategorizedModel(true);

    m_personsDelegate = new PersonsDelegate(this);

    m_personsView->setModel(m_personsProxyModel);
    m_personsView->setItemDelegate(m_personsDelegate);
    m_personsView->setCategoryDrawer(new KCategoryDrawerV3(m_personsView));

    m_personsProxyModel->sort(0);
}

void MainWindow::showContactDetails(QModelIndex index)
{
    PersonData *person = new PersonData(index.data(PersonsModel::UriRole).toString());
    m_personDetailsView->setPerson(person);

    kDebug() << index.data(PersonsModel::UriRole).toUrl();
}

void MainWindow::onSelectedContactsChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_FOREACH (const QModelIndex &index, selected.indexes()) {
        m_selectedContacts.append(index);
    }
    Q_FOREACH (const QModelIndex &index, deselected.indexes()) {
        m_selectedContacts.removeAll(index);
    }
}

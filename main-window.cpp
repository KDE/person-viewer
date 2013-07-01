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

#include <QLayout>
#include <QTimer>

#include <KCategoryDrawer>
#include <KDebug>
#include <KPixmapSequence>
#include <KPixmapSequenceWidget>

#include <KPeople/PersonsModel>
#include <KPeople/PersonData>
#include <KPeople/PersonsModelFeature>
#include <kpeople/widgets/persondetailsview.h>

using namespace KPeople;

MainWindow::MainWindow(QWidget *parent)
    : KMainWindow(parent),
    m_personsDelegate(0),
    m_personsModel(0),
    m_personsProxyModel(0)
{
    setupUi(this);

    m_personsModel = new PersonsModel(this);
    connect(m_personsModel, SIGNAL(modelInitialized()),
            this, SLOT(onPersonModelReady()));

    QList<PersonsModelFeature> features;
    features << PersonsModelFeature::emailModelFeature(true)
             << PersonsModelFeature::avatarModelFeature(true)
             << PersonsModelFeature::imModelFeature(true)
             << PersonsModelFeature::fullNameModelFeature(true);
    m_personsModel->startQuery(features);

    m_personsView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_personsView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    m_mergeList->setLayout(new QVBoxLayout());

    m_busyWidget = new KPixmapSequenceWidget(m_personsView);
    //apparently KPixmapSequence has only few sizes, 22 is one of them
    m_busyWidget->setSequence(KPixmapSequence("process-working", 22));

    //we need to wait for the layouts to set new geometries
    QTimer::singleShot(0, this, SLOT(positionBusyOverlay()));
}

MainWindow::~MainWindow()
{
}

void MainWindow::onPersonModelReady()
{
    m_personsProxyModel = new PersonsProxyModel(this);
    m_personsProxyModel->setSourceModel(m_personsModel);
    m_personsProxyModel->setDynamicSortFilter(true);
    m_personsProxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_personsProxyModel->setCategorizedModel(true);

    m_personsDelegate = new PersonsDelegate(this);

    m_personsView->setModel(m_personsProxyModel);
    m_personsView->setItemDelegate(m_personsDelegate);
    m_personsView->setCategoryDrawer(new KCategoryDrawerV3(m_personsView));

    m_personsProxyModel->sort(0);
    m_busyWidget->hide();

    connect(m_personsView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this, SLOT(onSelectedContactsChanged(QItemSelection,QItemSelection)));

    connect(m_mergeButton, SIGNAL(clicked(bool)), SLOT(onMergeButtonPressed()));

    connect(m_filterBar, SIGNAL(filterChanged(QString)),
            m_personsProxyModel, SLOT(setFilterFixedString(QString)));
}


void MainWindow::onSelectedContactsChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    const QModelIndexList &indexes = m_personsView->selectionModel()->selectedIndexes();

    if (indexes.size() == 1) {
        QString uri = indexes[0].data(PersonsModel::UriRole).toString();

        m_stackWidget->setCurrentIndex(1);
        PersonDataPtr person = PersonData::createFromUri(uri);

        m_detailsView->setPerson(person);
    } else {
        m_stackWidget->setCurrentIndex(0);
    }

    Q_FOREACH (const QModelIndex &index, selected.indexes()) {
        QString uri = index.data(PersonsModel::UriRole).toString();
        PersonDetailsView *details = m_cachedDetails.value(uri);
        if (!details) {
            details = new PersonDetailsView();
            details->setPerson(PersonData::createFromUri(uri));
            m_cachedDetails.insert(uri, details);
        }
        m_mergeList->layout()->addWidget(details);
    }

    Q_FOREACH (const QModelIndex &index, deselected.indexes()) {
        QString uri = index.data(PersonsModel::UriRole).toString();
        if (PersonDetailsView* cached = m_cachedDetails.take(uri)) {
            m_mergeList->layout()->removeWidget(cached);
            cached->deleteLater();
        }
        qDebug() << "removing" << uri;
    }
}

void MainWindow::onMergeButtonPressed()
{
    QList<QUrl> uris;
    Q_FOREACH (const QModelIndex &index, m_personsView->selectionModel()->selectedIndexes()) {
        uris << index.data(PersonsModel::UriRole).toUrl();
    }

    m_personsModel->createPersonFromUris(uris);
}

void MainWindow::positionBusyOverlay()
{
    m_busyWidget->setGeometry(m_personsView->size().width() / 2 - 11,
                              m_personsView->size().height() / 2 - 11,
                              22,
                              22);
}

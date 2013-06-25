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
#include "person-details-view.h"

#include <QLayout>
#include <QTimer>

#include <KCategoryDrawer>
#include <KDebug>
#include <KPixmapSequence>
#include <KPixmapSequenceWidget>

#include <kpeople/personsmodel.h>
#include <kpeople/persondata.h>
#include <kpeople/personsmodelfeature.h>

using namespace KPeople;

MainWindow::MainWindow(QWidget *parent)
    : KMainWindow(parent),
    m_personsDelegate(0),
    m_personsModel(0),
    m_personsProxyModel(0)
{
    setupUi(this);

    m_personsModel = new PersonsModel(0, PersonsModel::FeatureFullName
                                         | PersonsModel::FeatureEmails
                                         | PersonsModel::FeatureAvatars
                                         | PersonsModel::FeatureIM, this);
    connect(m_personsModel, SIGNAL(modelInitialized()),
            this, SLOT(onPersonModelReady()));

    m_personsView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_personsView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    m_personsDetailsView->setLayout(new QVBoxLayout());

    m_mergeButton->setVisible(false);

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

    if (m_personsView->selectionModel()->selectedIndexes().size() < 2) {
        m_mergeButton->setVisible(false);
    } else {
        m_mergeButton->setVisible(true);
    }
    QString uri;

    //add all new contacts
    Q_FOREACH (const QModelIndex &index, selected.indexes()) {
        uri = index.data(PersonsModel::UriRole).toString();
        //if it's a fake person, use the uri of the contact instead
        if (uri.left(10).compare("fakeperson") == 0) {
            //there must not exist a person with 0 contacts
            Q_ASSERT(index.model()->rowCount(index) > 0);
            uri = index.data(PersonsModel::ChildContactsUriRole).toList().first().toString();
        }
        PersonDetailsView *details = m_cachedDetails.value(uri);
        if (!details) {
            details = new PersonDetailsView();
            details->setPerson(new PersonData(uri, details));
            m_cachedDetails.insert(uri, details);
        }
        qDebug() << "adding" << uri;
        m_personsDetailsView->layout()->addWidget(details);
    }

    Q_FOREACH (const QModelIndex &index, deselected.indexes()) {
        uri = index.data(PersonsModel::UriRole).toString();
        //if it's a fake person, use the uri of the contact instead
        if (uri.left(10).compare("fakeperson") == 0) {
            //there must not exist a person with 0 contacts
            Q_ASSERT(index.model()->rowCount(index) > 0);
            uri = index.child(0, 0).data(PersonsModel::UriRole).toString();
        }
        if (PersonDetailsView* cached = m_cachedDetails.take(uri)) {
            cached->deleteLater();
        }
        qDebug() << "removing" << uri;
    }
}

void MainWindow::onMergeButtonPressed()
{
    m_personsModel->createPersonFromIndexes(m_personsView->selectionModel()->selectedIndexes());
}

void MainWindow::positionBusyOverlay()
{
    m_busyWidget->setGeometry(m_personsView->size().width() / 2 - 11,
                              m_personsView->size().height() / 2 - 11,
                              22,
                              22);
}

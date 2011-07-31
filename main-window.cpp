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
#include <Nepomuk/Vocabulary/NCO>
#include <Nepomuk/Vocabulary/PIMO>
#include <Nepomuk/Variant>

#include <QtGui/QFormLayout>

#include <KCategoryDrawer>
#include <KDebug>
#include <KFileDialog>

#include <libperson/person-object.h>
#include <libperson/persons-model.h>
#include <libperson/persons-proxy-model.h>

#include "main-window.h"
#include "persons-delegate.h"


MainWindow::MainWindow(QWidget* parent)
    : KMainWindow(parent),
    m_personsModel(0),
    m_personsProxyModel(0),
    m_personsDelegate(0)
{
    setupUi(this);

    m_personsModified = false;

    m_personsModel = new PersonsModel(PersonsModel::AlphabetCategories, this);
    m_personsProxyModel = new PersonsProxyModel(this);
    m_personsProxyModel->setSourceModel(m_personsModel);
    m_personsProxyModel->setDynamicSortFilter(true);
    m_personsProxyModel->setCategorizedModel(true);

    m_personsDelegate = new PersonsDelegate(this);

    m_personsView->setModel(m_personsProxyModel);
    m_personsView->setItemDelegate(m_personsDelegate);
    m_personsView->setCategoryDrawer(new KCategoryDrawerV3(m_personsView));

    m_personsProxyModel->sort(0);

    connect(m_personsView, SIGNAL(clicked(QModelIndex)),
            this, SLOT(showContactDetails(QModelIndex)));

    connect(m_personDetailsView, SIGNAL(contactPhotoOverlayClicked(QString)),
            this, SLOT(manageContactPicture(QString)));
}

MainWindow::~MainWindow()
{
    if (m_personsModified) {
        m_personsModel->savePersons();
    }
}

void MainWindow::showContactDetails(QModelIndex index)
{
    m_personDetailsView->setPerson(index.data(PersonsModel::ItemRole).value<PersonObject*>());
}

void MainWindow::manageContactPicture(const QString &button)
{
    if (button == QLatin1String("change-photo")) {
        KUrl image = KFileDialog::getImageOpenUrl(QDir::homePath(), this, i18n("Please select an image for your contact"));
        if (image.isValid()) {
            PersonObject *person = m_personsProxyModel->data(m_personsView->selectionModel()->currentIndex(), PersonsModel::ItemRole).value<PersonObject* >();
            kDebug() << "Got person with name" << person->label();
            person->setPhoto(image);
            m_personsModified = true;
            //this will only update the already existing person
            m_personsModel->insertPerson(person);
            m_personsProxyModel->sort(0);
        }
    }
    if (button == QLatin1String("remove-photo")) {
        PersonObject *person = m_personsProxyModel->data(m_personsView->selectionModel()->currentIndex(), PersonsModel::ItemRole).value<PersonObject* >();
        kDebug() << "Got person with name" << person->label();
        person->removePhoto();
        m_personsModified = true;
        m_personsModel->insertPerson(person);
        m_personsProxyModel->sort(0);
        m_personDetailsView->update();
    }
}

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

#include <KDebug>

#include "main-window.h"
#include "persons-model.h"


MainWindow::MainWindow(QWidget* parent)
    : KMainWindow(parent),
    m_personsModel(0)
{
    setupUi(this);

    m_personsModel = new PersonsModel();
    m_listView->setModel(m_personsModel);

    connect(m_listView, SIGNAL(clicked(QModelIndex)),
            this, SLOT(showContactDetails(QModelIndex)));
}

MainWindow::~MainWindow()
{

}

void MainWindow::showContactDetails(QModelIndex index)
{
    Nepomuk::Resource r(m_personsModel->data(index, PersonsModel::UriRole).toUrl());
    Nepomuk::Resource tag(r.property(Nepomuk::Vocabulary::PIMO::hasTag()).toResource());

    qDeleteAll(m_detailsFrame->children());

    QFormLayout *l = new QFormLayout(m_detailsFrame);

    QLabel *name = new QLabel(tag.property(Nepomuk::Vocabulary::PIMO::tagLabel()).toString(), m_detailsFrame);
    l->addRow(name);

    QStringList emails;
    QStringList phones;

    foreach(QUrl uri, r.property(Nepomuk::Vocabulary::PIMO::groundingOccurrence()).toUrlList()) {
        Nepomuk::Resource g(uri);
        if (g.hasProperty(Nepomuk::Vocabulary::NCO::hasEmailAddress())) {
            Nepomuk::Resource email(g.property(Nepomuk::Vocabulary::NCO::hasEmailAddress()).toResource());
            emails << email.property(Nepomuk::Vocabulary::NCO::emailAddress()).toStringList();
        }

        if (g.hasProperty(Nepomuk::Vocabulary::NCO::hasPhoneNumber())) {
            Nepomuk::Resource phone(g.property(Nepomuk::Vocabulary::NCO::hasPhoneNumber()).toResource());
            phones << phone.property(Nepomuk::Vocabulary::NCO::phoneNumber()).toStringList();
        }
    }

    kDebug() << emails << phones;

    int i = 0;
    foreach(QString email, emails) {
        QLabel *emailStringLabel = new QLabel(i18n("Email %1:").arg(++i), this);
        QLabel *emailLabel = new QLabel(email, m_detailsFrame);

        l->addRow(emailStringLabel, emailLabel);
    }

    i = 0;
    foreach(QString phone, phones) {
        QLabel *phoneStringLabel = new QLabel(i18n("Phone %1:").arg(++i), this);
        QLabel *phoneLabel = new QLabel(phone, m_detailsFrame);

        l->addRow(phoneStringLabel, phoneLabel);
    }

    m_detailsFrame->setLayout(l);
}

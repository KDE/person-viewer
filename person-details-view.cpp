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


#include "person-details-view.h"

#include <libperson/person-object.h>
#include <Nepomuk/Resource>
#include <Nepomuk/Vocabulary/NCO>
#include <Nepomuk/Variant>
#include <KDebug>
#include <QFormLayout>

#include "photo-overlay-event-filter.h"

using namespace Nepomuk::Vocabulary;

PersonDetailsView::PersonDetailsView(QWidget *parent) : QWidget(parent)
{
    setupUi(this);

    PhotoOverlayEventFilter *eventFilter = new PhotoOverlayEventFilter(m_contactPicture);
    connect(eventFilter, SIGNAL(buttonsClicked(QString)),
            this, SIGNAL(contactPhotoOverlayClicked(QString)));
}

PersonDetailsView::~PersonDetailsView()
{
}

void PersonDetailsView::setPerson(PersonObject* person)
{
    if (person->label().isEmpty()) {
        m_contactName->setText("<i>Unnamed person</i>");
    } else {
        m_contactName->setText(person->label());
    }

    Nepomuk::Resource personRes(person->uri());
    Nepomuk::Resource addressRes(personRes.property(NCO::hasPostalAddress()).toUrl());

    kDebug() << personRes.property(NCO::hasPostalAddress()).toUrl();

    QString address;

    if (addressRes.isValid()) {
        QString streetAddress = addressRes.property(NCO::streetAddress()).toString();
        QString postalCode = addressRes.property(NCO::postalcode()).toString();
        QString locality = addressRes.property(NCO::locality()).toString();
        QString country = addressRes.property(NCO::country()).toString();

        address = streetAddress.append("\n");
        address.append(postalCode).append(" ").append(locality).append("\n");
        address.append(country);
    }

    bool hasPicture = !person->photo().isNull();

    if (hasPicture) {
        m_contactPicture->setPixmap(person->photo());
        m_setPicButton->hide();
    } else {
        m_contactPicture->setPixmap(KIconLoader::global()->loadIcon("user-identity", KIconLoader::NoGroup, 64));
        m_setPicButton->show();
    }

    bool hasIM = !person->allIMContacts().isEmpty();
    kDebug() << "Has IM:" << hasIM;

    if (hasIM) {
        foreach(const QUrl& uri, person->allIMContacts()) {
            Nepomuk::Resource im(uri);
            m_contactPresence->setText(im.property(NCO::imStatus()).toString());
        }
        m_contactPresence->show();
        m_setIMContactButton->hide();
    } else {
        m_contactPresence->hide();
        m_setIMContactButton->show();
    }

    m_contactAddress->setText(address);

    //delete all that is in the area for emails and phones
    if (!m_contactsArea->children().isEmpty()) {
        qDeleteAll(m_contactsArea->children());
    }

    QFormLayout *layout = new QFormLayout(m_contactsArea);
    layout->setFormAlignment(Qt::AlignHCenter);

    foreach(const QUrl& uri, person->allContacts()) {
        kDebug() << "Grounding occurance uri:" << uri;
        Nepomuk::Resource g(uri);
        QList<QUrl> emails = g.property(NCO::hasEmailAddress()).toUrlList();
        if (!emails.isEmpty()) {
            kDebug() << "Found emails";
            int emailCounter = 0;
            foreach(const QUrl& emailUri, emails) {
                Nepomuk::Resource email(emailUri);
                layout->addRow(i18n("Email %1:", ++emailCounter), new QLabel(email.property(NCO::emailAddress()).toString(), m_contactsArea));
            }
        }

        QList<QUrl> phones = g.property(NCO::hasPhoneNumber()).toUrlList();
        if (!phones.isEmpty()) {
            kDebug() << "Found phone numbers";
            int phoneCounter = 0;
            foreach(const QUrl& phoneUri, phones) {
                Nepomuk::Resource phone(phoneUri);
                layout->addRow(i18n("Phone %1:", ++phoneCounter), new QLabel(phone.property(NCO::phoneNumber()).toString(), m_contactsArea));
            }
        }

        QList<QUrl> affiliations = g.property(NCO::hasAffiliation()).toUrlList();
        foreach(const QUrl& affUri, affiliations) {
            QList<QUrl> workPhones = Nepomuk::Resource(affUri).property(NCO::hasPhoneNumber()).toUrlList();
            int workPhoneCounter = 0;
            if (!workPhones.isEmpty()) {
                foreach(const QUrl& workPhoneUri, workPhones) {
                    Nepomuk::Resource workPhone(workPhoneUri);
                    layout->addRow(i18n("Work phone %1:", ++workPhoneCounter), new QLabel(workPhone.property(NCO::phoneNumber()).toString(), m_contactsArea));
                }
            }
        }
    }

    m_contactsArea->setLayout(layout);
}


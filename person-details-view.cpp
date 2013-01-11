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

#include <Nepomuk/Resource>
#include <Nepomuk/Vocabulary/NCO>
#include <Nepomuk/Variant>
#include <KDebug>
#include <KIconLoader>

#include <QVBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QAction>
#include <QToolButton>
#include <QModelIndex>

#include <kpeople/persondata.h>
#include <kpeople/persons-model.h>

using namespace Nepomuk::Vocabulary;

PersonDetailsView::PersonDetailsView(QWidget *parent)
    : QWidget(parent)
{
    m_mainLayout = new QVBoxLayout(this);
    m_contactPixmap = new QLabel(this);
    m_contactNameLabel = new QLabel(this);
    QFont f;
    f.setPixelSize(18);
    m_contactNameLabel->setFont(f);
//     m_contactIdLabel = new QLabel(this);
    m_contactStatusLabel = new QLabel(this);

    QHBoxLayout *namePresenceLayout = new QHBoxLayout(this);
    namePresenceLayout->addWidget(m_contactNameLabel);
    namePresenceLayout->addWidget(m_contactStatusLabel);

    m_contactsListWidget = new QWidget(this);

    m_mainLayout->addWidget(m_contactPixmap);
    m_mainLayout->addLayout(namePresenceLayout);
    m_mainLayout->addWidget(m_contactsListWidget);

    m_mainLayout->addSpacerItem(new QSpacerItem(32, 32, QSizePolicy::Minimum, QSizePolicy::Expanding));
}

PersonDetailsView::~PersonDetailsView()
{
}

void PersonDetailsView::setPerson(PersonData *person)
{
    m_person = person;


//     PersonActions* actions = new PersonActions(&m_person);
//     actions->setPerson(&m_person);
//     QToolButton* b = new QToolButton;
//     b->setText("Actions");
//     QMenu* m = new QMenu(b);
//     m->addActions(actions->actions());
//     b->setPopupMode(QToolButton::MenuButtonPopup);
//     b->setMenu(m);
//     l->addRow("actions:", b);
}

void PersonDetailsView::drawStuff()
{
    QPixmap avatar;
    QString avatarPath = m_person->avatar().toLocalFile();
    if (avatarPath.isEmpty()) {
        avatar.load("/home/mck182/Downloads/dummy-avatar.png");
    } else {
        avatar.load(avatarPath);
    }
    m_contactPixmap->setPixmap(avatar);
    m_contactNameLabel->setText(m_person->name());
    m_contactStatusLabel->setPixmap(QPixmap());

//     m_contactIdLabel->setText(m_person->contacts().join(", "));

    qDeleteAll(m_contactsListWidget->children());
    m_contactsListWidget->setLayout(new QVBoxLayout(m_contactsListWidget));

    QFont f;
    f.setPixelSize(16);

    if (!m_person->personIndex().data(PersonsModel::EmailRole).isNull()) {
        kDebug() << "Email:" << m_person->personIndex().data(PersonsModel::EmailRole);
        QLabel *title = new QLabel("Emails", m_contactsListWidget);
        title->setFont(f);
        m_contactsListWidget->layout()->addWidget(title);

        if (m_person->isPerson()) {
            QVariantList l = m_person->personIndex().data(PersonsModel::EmailRole).toList();
            Q_FOREACH(const QVariant &v, l) {
                QLabel *name = new QLabel(v.toString(), m_contactsListWidget);
                name->setToolTip(v.toString());
                m_contactsListWidget->layout()->addWidget(name);
            }
        } else {
            QString c = m_person->personIndex().data(PersonsModel::EmailRole).toString();
            QLabel *name = new QLabel(c, m_contactsListWidget);
            name->setToolTip(c);
            m_contactsListWidget->layout()->addWidget(name);
//             Q_FOREACH(QAction *action, m_person->personIndex().data(PersonsModel::ContactActionsRole).value<QList<QAction*> >()) {
//                 if (action->property("contactId").toString() == c) {
//                     QToolButton *actionButton = new QToolButton(m_contactsListWidget);
//                     actionButton->setIcon(action->icon());
//                     connect(actionButton, SIGNAL(pressed()),
//                             action, SLOT(trigger()));
//                     m_contactsListWidget->layout()->addWidget(actionButton);
//                 }
//             }
        }
    }

    if (!m_person->personIndex().data(PersonsModel::IMRole).isNull()) {
        kDebug() << "IM Accounts:" << m_person->personIndex().data(PersonsModel::IMRole);
        QLabel *title = new QLabel("IM Accounts", m_contactsListWidget);
        title->setFont(f);
        m_contactsListWidget->layout()->addWidget(title);

        QVariant imaccounts = m_person->personIndex().data(PersonsModel::IMRole);
        QVariant imtypes = m_person->personIndex().data(PersonsModel::IMAccountTypeRole);

        if (m_person->isPerson()) {
            QVariantList l = imtypes.toList();
            Q_FOREACH(const QVariant &v, l) {
                QLabel *name = new QLabel(accountTypeDisplayString(v.toString()), m_contactsListWidget);
                name->setToolTip(v.toString());
                m_contactsListWidget->layout()->addWidget(name);
                m_contactStatusLabel->setPixmap(iconForPresence(findMostOnlinePresence(
                    m_person->personIndex().data(PersonsModel::StatusRole).toList())));
            }
        } else {
            QString c = imtypes.toString();
            QString contactId = imaccounts.toString();
            QLabel *name = new QLabel(accountTypeDisplayString(c), m_contactsListWidget);
            name->setToolTip(c);
            m_contactsListWidget->layout()->addWidget(name);
            m_contactStatusLabel->setPixmap(iconForPresence(m_person->personIndex().data(PersonsModel::StatusRole).toString()));
//             Q_FOREACH(QAction *action, m_person->personIndex().data(PersonsModel::ContactActionsRole).value<QList<QAction*> >()) {
//                 if (action->property("contactId").toString() == imaccounts) {
//                     QToolButton *actionButton = new QToolButton(m_contactsListWidget);
//                     actionButton->setIcon(action->icon());
//                     connect(actionButton, SIGNAL(pressed()),
//                             action, SLOT(trigger()));
//                     m_contactsListWidget->layout()->addWidget(actionButton);
//                 }
//             }
        }
    }
    if (!m_person->personIndex().data(PersonsModel::PhoneRole).isNull()) {
        QLabel *title = new QLabel("Phone Numbers", m_contactsListWidget);
        title->setFont(f);
        m_contactsListWidget->layout()->addWidget(title);

        QVariant phoneNumbers = m_person->personIndex().data(PersonsModel::PhoneRole);

        if (m_person->isPerson()) {
            QVariantList l = phoneNumbers.toList();
            Q_FOREACH(const QVariant &v, l) {
                QLabel *name = new QLabel(v.toString(), m_contactsListWidget);
                name->setToolTip(v.toString());
                m_contactsListWidget->layout()->addWidget(name);
            }
        } else {
            QString c = phoneNumbers.toString();
            QLabel *name = new QLabel(c, m_contactsListWidget);
            name->setToolTip(c);
            m_contactsListWidget->layout()->addWidget(name);
        }
    }
}

QString PersonDetailsView::accountTypeDisplayString(const QString &accountType) const
{
    if (accountType == QLatin1String("facebook")) {
        return i18n("Facebook Chat");
    }
    if (accountType == QLatin1String("google-talk")) {
        return i18n("GTalk");
    }
    if (accountType == QLatin1String("haze-yahoo")) {
        return i18n("Yahoo! Messenger");
    }
    if (accountType == QLatin1String("msn")) {
        return i18n("MSN Messenger");
    }
    if (accountType == QLatin1String("haze-icq")) {
        return i18n("ICQ");
    }
    if (accountType == QLatin1String("jabber")) {
        return i18n("Jabber");
    }
    if (accountType == QLatin1String("haze-gadugadu")) {
        return i18n("GaduGadu");
    }

    return accountType;
}

QPixmap PersonDetailsView::iconForPresence(const QString &presenceString)
{
    KIconLoader il;
    if (presenceString == "available") {
        return il.loadIcon("user-online", KIconLoader::Toolbar);
    }
    if (presenceString == "away") {
        return il.loadIcon("user-away", KIconLoader::Toolbar);
    }
    if (presenceString == "busy" || presenceString == "dnd") {
        return il.loadIcon("user-busy", KIconLoader::Toolbar);
    }
    if (presenceString == "xa") {
        return il.loadIcon("user-online", KIconLoader::Toolbar);
    }
    if (presenceString == "offline") {
        return il.loadIcon("user-offline", KIconLoader::Toolbar);
    }

    return QPixmap();
}

QString PersonDetailsView::findMostOnlinePresence(const QVariantList &presences) const
{
    if (presences.contains("available")) {
        return "available";
    }
    if (presences.contains("away")) {
        return "away";
    }
    if (presences.contains("busy") || presences.contains("dnd")) {
        return "busy";
    }
    if (presences.contains("xa")) {
        return "xa";
    }

    return "offline";
}

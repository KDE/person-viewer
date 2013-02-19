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


#include "person-details-view.h"

#include <Nepomuk/Resource>
#include <Nepomuk/Vocabulary/NCO>
#include <Nepomuk/Variant>
#include <KDebug>
#include <KIconLoader>
#include <KStandardDirs>

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

    //initialize the PresenceModel so when user selects an actual contact, we already have presences ready
    m_person = new PersonData(QString(), this);
}

PersonDetailsView::~PersonDetailsView()
{
}

void PersonDetailsView::setPerson(PersonData *person)
{
    disconnect(m_person, SIGNAL(dataChanged()), this, SLOT(drawStuff()));

    m_person->deleteLater();

    m_person = person;

    connect(m_person, SIGNAL(dataChanged()), this, SLOT(drawStuff()));

    drawStuff();

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
        avatar.load(KStandardDirs::locate("data", "person-viewer/dummy_avatar.png"));
    } else {
        avatar.load(avatarPath);
    }
    m_contactPixmap->setPixmap(avatar);
    m_contactNameLabel->setText(m_person->name());
    m_contactStatusLabel->setPixmap(iconForPresence(m_person->status()));

    qDeleteAll(m_contactsListWidget->children());
    m_contactsListWidget->setLayout(new QVBoxLayout(m_contactsListWidget));

    QFont f;
    f.setPixelSize(16);

    if (!m_person->emails().isEmpty()) {
        kDebug() << "Emails:" << m_person->emails();
        QLabel *title = new QLabel("Emails", m_contactsListWidget);
        title->setFont(f);
        m_contactsListWidget->layout()->addWidget(title);

        Q_FOREACH (const QString &email, m_person->emails()) {
            QLabel *emailLabel = new QLabel(email, m_contactsListWidget);
            m_contactsListWidget->layout()->addWidget(emailLabel);
        }
    }

    if (!m_person->imAccounts().isEmpty()) {
        kDebug() << "IM Accounts:" << m_person->imAccounts();
        QLabel *title = new QLabel("IM Accounts", m_contactsListWidget);
        title->setFont(f);
        m_contactsListWidget->layout()->addWidget(title);

        QStringList imAccounts = m_person->imAccounts();

        for (int i = 0; i < imAccounts.size(); i++) {
            QLabel *name = new QLabel(accountTypeDisplayString(imAccounts.at(i++)), m_contactsListWidget);

            QString imNickname = imAccounts.at(i++);
            QString imID = imAccounts.at(i);

            name->setToolTip(QString("%1 (%2)").arg(imNickname, imID));
            m_contactsListWidget->layout()->addWidget(name);
//             m_contactStatusLabel->setPixmap(iconForPresence(findMostOnlinePresence(
//                 m_person->personIndex().data(PersonsModel::StatusRole).toList())));
        }
    }

    if (!m_person->phones().isEmpty()) {
        kDebug() << "Phones:" << m_person->phones();
        QLabel *title = new QLabel("Phones", m_contactsListWidget);
        title->setFont(f);
        m_contactsListWidget->layout()->addWidget(title);

        Q_FOREACH (const QString &phone, m_person->phones()) {
            QLabel *phoneLabel = new QLabel(phone, m_contactsListWidget);
            m_contactsListWidget->layout()->addWidget(phoneLabel);
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

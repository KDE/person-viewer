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
#include "facebook-connector.h"

#include <Nepomuk/Resource>
#include <Nepomuk/Vocabulary/NCO>
#include <Nepomuk/Variant>
#include <KDebug>
#include <KIconLoader>
#include <KStandardDirs>
#include <KIcon>

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
    m_contactStatusLabel = new QLabel(this);
    m_contactBirthdayLabel = new QLabel(this);

    QGridLayout *namePresenceLayout = new QGridLayout(this);
    namePresenceLayout->addWidget(m_contactPixmap, 0, 0, 2, 1);
    namePresenceLayout->addWidget(m_contactNameLabel, 0, 1);
    namePresenceLayout->addWidget(m_contactStatusLabel, 0, 2);
    namePresenceLayout->addWidget(m_contactBirthdayLabel, 1, 1);
    namePresenceLayout->addItem(new QSpacerItem(1, 1, QSizePolicy::Expanding), 0, 3);

    QFrame *line = new QFrame(this);
    line->setFrameShape(QFrame::HLine);

    namePresenceLayout->addWidget(line, 2, 0, 1, 4);

    m_contactsListWidget = new QWidget(this);

//     m_mainLayout->addWidget(m_contactPixmap);
    m_mainLayout->addLayout(namePresenceLayout);
    m_mainLayout->addWidget(m_contactsListWidget);

    m_facebookPostWidget = new FacebookConnector(this);

    m_mainLayout->addWidget(m_facebookPostWidget);
    m_facebookPostWidget->hide();

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
}

void PersonDetailsView::drawStuff()
{
    kDebug();
    QPixmap avatar;
    QString avatarPath = m_person->avatar().toLocalFile();
    if (!avatarPath.isEmpty()) {
        avatar.load(avatarPath);
    }

    if (avatar.isNull()) {
        avatar.load(KStandardDirs::locate("data", "person-viewer/dummy_avatar.png"));
    }
    m_contactPixmap->setPixmap(avatar.scaled(96, 96, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    m_contactNameLabel->setText(m_person->name());
    m_contactStatusLabel->setPixmap(iconForPresence(m_person->status()));
//     m_contactBirthdayLabel->setText(m_person->birthday().date().toString());

    qDeleteAll(m_contactsListWidget->children());
    QGridLayout *layout = new QGridLayout(m_contactsListWidget);
    layout->setColumnMinimumWidth(0, 22);

    m_contactsListWidget->setLayout(layout);
    m_facebookPostWidget->clear();
    m_facebookPostWidget->hide();

    QFont f;
    f.setPixelSize(16);

    if (!m_person->emails().isEmpty()) {
        kDebug() << "Emails:" << m_person->emails();
        QLabel *icon = new QLabel(this);
        icon->setPixmap(KIcon("mail-message").pixmap(KIconLoader::SizeSmall, KIconLoader::SizeSmall));
        QLabel *title = new QLabel("Emails", m_contactsListWidget);
        title->setFont(f);
        int row = layout->rowCount();
        layout->addWidget(icon, row, 0);
        layout->addWidget(title, row, 1);
//         layout->addWidget(title, );

        Q_FOREACH (const QString &email, m_person->emails()) {
            QLabel *emailLabel = new QLabel(email, m_contactsListWidget);
            layout->addWidget(emailLabel, ++row, 1);
        }
    }

    if (!m_person->imAccounts().isEmpty()) {
        kDebug() << "IM Accounts:" << m_person->imAccounts();
        QLabel *icon = new QLabel(this);
        icon->setPixmap(KIcon("telepathy-kde").pixmap(KIconLoader::SizeSmall, KIconLoader::SizeSmall));
        QLabel *title = new QLabel("IM Accounts", m_contactsListWidget);
        title->setFont(f);
        int row = layout->rowCount();
        layout->addWidget(icon, row, 0);
        layout->addWidget(title, row, 1);
//         m_contactsListWidget->layout()->addWidget(title);

        QStringList imAccounts = m_person->imAccounts();

        for (int i = 0; i < imAccounts.size(); i++) {
            bool facebookAccount = false;
            if (imAccounts.at(i) == "facebook") {
                facebookAccount = true;
            }

            QLabel *name = new QLabel(accountTypeDisplayString(imAccounts.at(i++)), m_contactsListWidget);

            QString imNickname = imAccounts.at(i++);
            QString imID = imAccounts.at(i);

            if (facebookAccount) {
                //@chat.facebook.com
                kDebug() << imID << imID.mid(1, imID.length() - 19);
                m_facebookPostWidget->setUserId(imID.mid(1, imID.length() - 19));
                m_facebookPostWidget->show();
            }

            name->setToolTip(QString("%1 (%2)").arg(imNickname, imID));
            layout->addWidget(name, ++row, 1);
//             m_contactsListWidget->layout()->addWidget(name);
//             m_contactStatusLabel->setPixmap(iconForPresence(findMostOnlinePresence(
//                 m_person->personIndex().data(PersonsModel::StatusRole).toList())));
        }
    }

    if (!m_person->phones().isEmpty()) {
        kDebug() << "Phones:" << m_person->phones();
        QLabel *icon = new QLabel(this);
        icon->setPixmap(KIcon("phone").pixmap(KIconLoader::SizeSmall, KIconLoader::SizeSmall));
        QLabel *title = new QLabel("Phones", m_contactsListWidget);
        title->setFont(f);
        int row = layout->rowCount();
        layout->addWidget(icon, row, 0);
        layout->addWidget(title, row, 1);

        Q_FOREACH (const QString &phone, m_person->phones()) {
            QLabel *phoneLabel = new QLabel(phone, m_contactsListWidget);
            layout->addWidget(phoneLabel, ++row, 1);
        }
    }

//     if (!m_person->contactUID().isEmpty()) {
//         m_facebookPostWidget->setUserId(m_person->contactUID());
//         m_facebookPostWidget->show();
//     }
//
//     kDebug() << m_person->birthday().date().toString();

    layout->addItem(new QSpacerItem(150, 1, QSizePolicy::Expanding, QSizePolicy::Minimum), 5, 1);

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

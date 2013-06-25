/*
    Copyright (C) 2013  David Edmundson <D.Edmundson@lboro.ac.uk>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "im-details-widget.h"

#include <QVBoxLayout>
#include <QLabel>

#include <KDebug>
#include <KIcon>
#include <KLocalizedString>

#include <kpeople/persondata.h>

using namespace KPeople;

IMDetailsWidget::IMDetailsWidget(QWidget* parent):
    AbstractPersonDetailsWidget(parent)
{

    setTitle(i18n("Instant Messaging"));
    setIcon(KIcon("telepathy-kde"));
}


void IMDetailsWidget::setPerson(PersonData* person)
{
    if(person->imAccounts().size() == 0) {
        setActive(false);
    } else {
        setActive(true);
    }

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0,0,0,0);
    setLayout(layout);
    //personData->imCaccounts is awful

    QStringList imAccounts = person->imAccounts();
    for (int i = 0; i < imAccounts.size(); i++) {
        QLabel *name = new QLabel(accountTypeDisplayString(imAccounts.at(i++)), this);

        QString imNickname = imAccounts.at(i++);
        QString imID = imAccounts.at(i);


        name->setToolTip(QString("%1 (%2)").arg(imNickname, imID));
        layout->addWidget(name);
    }
}

QString IMDetailsWidget::accountTypeDisplayString(const QString &accountType) const
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


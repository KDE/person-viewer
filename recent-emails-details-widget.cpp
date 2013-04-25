/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2013  David Edmundson <email>

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


#include "recent-emails-details-widget.h"

#include <QTreeWidget>
#include <QVBoxLayout>

#include <KLocalizedString>
#include <KIcon>
#include <KToolInvocation>

#include <kpeople/persondata.h>

#include <Nepomuk2/Resource>
#include <Nepomuk2/Query/Query>
#include <Nepomuk2/ResourceManager>
#include <Nepomuk2/ResourceWatcher>
#include <Nepomuk2/Vocabulary/PIMO>
#include <Nepomuk2/Vocabulary/NCO>
#include <Nepomuk2/Vocabulary/NIE>
#include <Soprano/Model>
#include <Soprano/QueryResultIterator>


RecentEmailsDetailsWidget::RecentEmailsDetailsWidget(QWidget* parent): AbstractPersonDetailsWidget(parent)
{
    setTitle(i18n("Recent Emails"));
    setIcon(KIcon("mail-message"));

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0,0,0,0);
    m_tableWidget = new QTreeWidget(this);
    m_tableWidget->setRootIsDecorated(false);
    m_tableWidget->setHeaderHidden(true);

    connect(m_tableWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), SLOT(onItemDoubleClicked(QTreeWidgetItem*)));

    layout->addWidget(m_tableWidget);
    setLayout(layout);
}


void RecentEmailsDetailsWidget::setPerson(PersonData* personData)
{
    QString query = QString::fromUtf8("select DISTINCT ?uri ?isRead ?subject ?messageDate ?akonadiUrl WHERE { ?uri a nmo:Email. ?uri nmo:from <%1>. ?uri nmo:isRead ?isRead. ?uri nmo:messageSubject ?subject. ?uri nmo:sentDate ?messageDate. ?uri nie:url ?akonadiUrl} LIMIT 20").arg(personData->uri());

    Soprano::Model *model = Nepomuk2::ResourceManager::instance()->mainModel();
    Soprano::QueryResultIterator it = model->executeQuery(query, Soprano::Query::QueryLanguageSparqlNoInference);

    m_tableWidget->clear();

    while (it.next()) {
        QString uri = it[0].uri().toString();
        bool isRead = it[1].toString() == "1";
        QString subject = it[2].toString();
        QString time = it[3].toString();
        QString akonadiUrl = it[4].toString();

        qDebug() << isRead << subject << time;
        QTreeWidgetItem *item = new QTreeWidgetItem(QStringList() << subject << time);
        item->setData(0, Qt::UserRole, akonadiUrl);
        item->setIcon(0, isRead ? KIcon("mail-message-read") : KIcon("mail-message-unread"));
        m_tableWidget->addTopLevelItem(item);
    }
    setActive(m_tableWidget->model()->rowCount() > 0);
}

void RecentEmailsDetailsWidget::onItemDoubleClicked(QTreeWidgetItem* item)
{
    QString akonadiUrl = item->data(0, Qt::UserRole).toString();

    if (!akonadiUrl.isEmpty()) {
        KToolInvocation::kdeinitExec("kmail", QStringList() << "--view" << akonadiUrl);
    }
}




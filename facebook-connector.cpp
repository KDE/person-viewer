/*
    <one line to give the library's name and an idea of what it does.>
    Copyright (C) 2013  Martin Klapetek <email>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/


#include "facebook-connector.h"
#include <libkfbapi/postslistjob.h>

#include <KDebug>
#include <KPixmapSequenceWidget>
#include <KPixmapSequence>
#include <KLocalizedString>
#include <KLocale>
#include <KIcon>
#include <KStandardDirs>
#include <KConfigGroup>
#include <KLocalizedString>

#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QDir>

#include <kpeople/persondata.h>

FacebookConnector::FacebookConnector(QWidget *parent)
    : AbstractPersonDetailsWidget(parent)
{
    setTitle(i18n("Facebook"));
    setIcon(KIcon("im-facebook"));

    QDir d(KGlobal::dirs()->saveLocation("config", QString(), false));

    QStringList akonadiFacebookConfigFileNames = d.entryList(QStringList() << "akonadi_facebook_resource*", QDir::Files);
    if (! akonadiFacebookConfigFileNames.isEmpty()) {
        KConfig akonadiFacebookConfig(d.entryList(QStringList() << "akonadi_facebook_resource*", QDir::Files).last());
        KConfigGroup afGroup = akonadiFacebookConfig.group("Authentication");

        m_accessToken = afGroup.readEntry("AccessToken");
    }
    QGridLayout *mainLayout = new QGridLayout(this);
    setLayout(mainLayout);
    //     mainLayout->setColumnStretch(0, 1);
    //     mainLayout->setColumnStretch(1, 2);
    mainLayout->setColumnMinimumWidth(0, 22);

    QLabel *facebookIcon = new QLabel(this);
    facebookIcon->setPixmap(KIcon("facebookresource").pixmap(KIconLoader::SizeSmall, KIconLoader::SizeSmall));

    mainLayout->addWidget(facebookIcon, 1, 0);

    //     QVBoxLayout *layout = new QVBoxLayout(this);
    //     mainLayout->addLayout(layout);

    QFont f;
    f.setPixelSize(18);

    QFrame *line = new QFrame(this);
    line->setFrameShape(QFrame::HLine);

    QLabel *title = new QLabel("Facebook Profile", this);
    title->setFont(f);

    f.setPixelSize(16);
    m_lastPostTitle = new QLabel(this);
    m_lastPostTitle->setFont(f);

    m_post = new QLabel(this);
    m_post->setWordWrap(true);
    m_post->hide();

    m_busyWidget = new KPixmapSequenceWidget(this);
    //apparently KPixmapSequence has only few sizes, 22 is one of them
    m_busyWidget->setSequence(KPixmapSequence("process-working", 22));
    m_busyWidget->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    m_busyWidget->hide();

    m_profileLink = new QLabel(this);
    m_profileLink->setTextInteractionFlags(Qt::LinksAccessibleByMouse | Qt::TextSelectableByMouse);

    mainLayout->addWidget(line, 0, 0, 1, 2);
    mainLayout->addWidget(title, 1, 1);
    mainLayout->addWidget(m_lastPostTitle, 2, 1);
    mainLayout->addWidget(m_busyWidget, 3, 0);
    mainLayout->addWidget(m_post, 3, 1);
    mainLayout->addWidget(m_profileLink, 4, 1);
    mainLayout->addItem(new QSpacerItem(150, 1, QSizePolicy::Expanding, QSizePolicy::Minimum), 5, 1);
}

void FacebookConnector::setPerson(PersonData* person)
{
    QString userId;

    userId = person->contactId();

    //also search IM accounts for anything Facebook related
    QStringList imAccounts = person->imAccounts();
    for (int i=0;i<imAccounts.size();i+=3) {
        if (imAccounts[i] == "facebook") {
            userId = imAccounts[i+1];
        }
    }

    if (!userId.isEmpty()) {
        setActive(true);
        setUserId(userId);
    } else {
        setActive(false);
    }
}

void FacebookConnector::setUserId(const QString &userId)
{
    if (m_accessToken.isEmpty()) {
        return;
    }
    if (userId == m_userId) {
        return;
    }

    m_userId = userId;

    if (!m_runningJob.isNull()) {
        m_runningJob->kill();
    }

    m_busyWidget->show();
    m_post->show();
    m_post->setText(i18n("Fetching last Facebook post..."));
    m_profileLink->setText("<a href=\"https://facebook.com/" + userId + "\">Open Facebook profile...</a>");

    KFbAPI::PostsListJob *job = new KFbAPI::PostsListJob(userId, m_accessToken, this);
    job->setProperty("userId", userId);
    connect(job, SIGNAL(result(KJob*)), this, SLOT(lastPostFetched(KJob*)));
    job->start();

    m_runningJob = job;
}

void FacebookConnector::lastPostFetched(KJob *job)
{
    m_busyWidget->hide();
    KFbAPI::PostsListJob *postsJob = qobject_cast<KFbAPI::PostsListJob*>(job);

    QString userId = postsJob->property("userId").toString();
    QString postMessage;
    QString postDate;

    Q_FOREACH(KFbAPI::PostInfo post, postsJob->posts()) {
        if (!post.message().isEmpty()) {
            postMessage = post.message();
            postDate = KGlobal::locale()->formatDateTime(post.createdTime(), KLocale::FancyLongDate);
            break;
        }
    }

    kDebug() << postMessage << postDate;

    if (postMessage.isEmpty()) {
        postMessage = QString("(empty)");
    }
    m_post->setText(postMessage);
    m_lastPostTitle->setText(QString("Last Post (from %1)").arg(postDate));
}

void FacebookConnector::clear()
{
    m_userId = QString();
    m_busyWidget->hide();
    m_post->hide();
}

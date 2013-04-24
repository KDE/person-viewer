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


#ifndef FACEBOOK_CONNECTOR_H
#define FACEBOOK_CONNECTOR_H

#include <QWidget>
#include <QPointer>

#include <KJob>

#include "abstractpersondetailswidget.h"

class QLabel;
class KPixmapSequenceWidget;

class FacebookConnector : public AbstractPersonDetailsWidget
{
    Q_OBJECT
public:
    FacebookConnector(QWidget *parent = 0);

    void setPerson(PersonData *person);

Q_SIGNALS:
    void lastPostForUserReady(const QString &userId, const QString &post);

public Q_SLOTS:
    void lastPostFetched(KJob *job);

private:
    void setUserId(const QString &userId);
    void clear();

    QString m_accessToken;
    QString m_userId;
    QPointer<KJob> m_runningJob;
    KPixmapSequenceWidget *m_busyWidget;
    QLabel *m_lastPostTitle;
    QLabel *m_post;
    QLabel *m_profileLink;
};

#endif // FACEBOOK_CONNECTOR_H

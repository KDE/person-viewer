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


#ifndef PERSON_DETAILS_VIEW_H
#define PERSON_DETAILS_VIEW_H

#include <QWidget>
#include <QVariantList>

class FacebookConnector;
class QLabel;
class PersonData;
class QVBoxLayout;
class AbstractPersonDetailsWidget;

class PersonDetailsView : public QWidget
{
    Q_OBJECT
public:
    PersonDetailsView(QWidget *parent = 0);
    virtual ~PersonDetailsView();

    void setPerson(PersonData *person);

public Q_SLOTS:
    void drawStuff();

private:
    QPixmap iconForPresence(const QString &presenceString);

    PersonData *m_person;
    QVBoxLayout *m_mainLayout;

    QLabel *m_contactPixmap;
    QLabel *m_contactNameLabel;
    QLabel *m_contactBirthdayLabel;
    QLabel *m_contactStatusLabel;
    QWidget *m_contactsListWidget;

    AbstractPersonDetailsWidget *m_emailDetailsWidget;
    AbstractPersonDetailsWidget *m_imDetailsWidget;
    AbstractPersonDetailsWidget *m_phoneDetailsWidget;

    FacebookConnector *m_facebookPostWidget;
};

#endif // PERSON_DETAILS_VIEW_H

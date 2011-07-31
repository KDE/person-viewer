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


#ifndef PERSON_DETAILS_VIEW_H
#define PERSON_DETAILS_VIEW_H

#include <QWidget>

#include "ui_person-details-view.h"

class PersonObject;

class PersonDetailsView : public QWidget, Ui::PersonDetailsView
{
    Q_OBJECT
public:
    PersonDetailsView(QWidget *parent = 0);
    virtual ~PersonDetailsView();

    void setPerson(PersonObject *person);

Q_SIGNALS:
    void contactPhotoOverlayClicked(const QString &button);
};

#endif // PERSON_DETAILS_VIEW_H

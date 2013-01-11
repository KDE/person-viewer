/*
    <one line to give the library's name and an idea of what it does.>
    Copyright (C) 2012  Martin Klapetek <email>

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


#ifndef PERSONS_LIST_VIEW_H
#define PERSONS_LIST_VIEW_H

#include <KCategorizedView>

class PersonsListView : public KCategorizedView
{
Q_OBJECT

public:
    PersonsListView(QWidget *parent = 0);
    virtual ~PersonsListView();

Q_SIGNALS:
    void selectedContacts(const QItemSelection &selected, const QItemSelection &deselected);

protected Q_SLOTS:
    void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
};

#endif // PERSONS_LIST_VIEW_H

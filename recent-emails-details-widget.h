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


#ifndef RECENT_EMAILS_DETAILS_WIDGET_H
#define RECENT_EMAILS_DETAILS_WIDGET_H

#include <abstract-person-details-widget.h>

class QTreeWidget;
class QTreeWidgetItem;

class RecentEmailsDetailsWidget : public AbstractPersonDetailsWidget
{
Q_OBJECT
public:
    explicit RecentEmailsDetailsWidget(QWidget* parent = 0);
    virtual void setPerson(PersonData* personData);

private slots:
    void onItemDoubleClicked(QTreeWidgetItem *item);
private:
    QTreeWidget *m_tableWidget;
};

#endif // RECENT_EMAILS_DETAILS_WIDGET_H

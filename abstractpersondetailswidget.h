/*
    <one line to give the program's name and a brief idea of what it does.>
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


#ifndef ABSTRATPERSONDETAILSWIDGET_H
#define ABSTRATPERSONDETAILSWIDGET_H

#include <QWidget>

#include <KIcon>

class PersonData;
class AbstractPersonDetailsWidgetPrivate;

class AbstractPersonDetailsWidget : public QWidget
{

public:
    explicit AbstractPersonDetailsWidget(QWidget *parent = 0);
    virtual ~AbstractPersonDetailsWidget();

    virtual void setPerson(PersonData *personData) = 0; //or put this in ctor? I'd prefer it I think...
    QString title() const;
    KIcon icon() const;

protected:
    void setTitle(const QString &title);
    void setIcon(const KIcon &icon);
//    PersonData* personData();

private:
    AbstractPersonDetailsWidgetPrivate * const d_ptr;
    Q_DECLARE_PRIVATE(AbstractPersonDetailsWidget);
};

#endif // ABSTRATPERSONDETAILSWIDGET_H

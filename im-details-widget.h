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


#ifndef IM_DETAILS_WIDGET_H
#define IM_DETAILS_WIDGET_H

#include "abstract-person-details-widget.h"

class IMDetailsWidget : public AbstractPersonDetailsWidget
{
public:
    IMDetailsWidget(QWidget *parent=0);
    virtual void setPerson(PersonData*);
private:
    QString accountTypeDisplayString(const QString &accountType) const;
};

#endif // IM_DETAILS_WIDGET_H

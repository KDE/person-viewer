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


#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <KDE/KMainWindow>
#include <KXmlGuiWindow>
#include <KAction>

#include "ui_main-window.h"

class MergingWidget;
class QItemSelection;
class IMPersonsModel;
class PIMPersonsModel;
class PersonsProxyModel;
class PersonsDelegate;
class PersonsModel;
class PersonsProxyModel;

class MainWindow : public KMainWindow, Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public Q_SLOTS:
    void showContactDetails(QModelIndex index);

private Q_SLOTS:
    void onPersonModelReady();
    void onSelectedContactsChanged(const QItemSelection &selected, const QItemSelection &deselected);

private:
    PersonsDelegate     *m_personsDelegate;
    PersonsModel        *m_personsModel;
    PersonsProxyModel   *m_personsProxyModel;
    bool                 m_personsModified;
    QList<QModelIndex>   m_selectedContacts;
};

#endif // MAIN_WINDOW_H

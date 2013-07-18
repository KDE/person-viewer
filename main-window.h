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


#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <KDE/KMainWindow>
#include <KXmlGuiWindow>
#include <KAction>

#include "ui_main-window.h"

namespace KPeople {
    class PersonsModel;
    class PersonDetailsView;
}
class KPixmapSequenceWidget;
class MergingWidget;
class QItemSelection;
class PersonsDelegate;
class PersonsProxyModel;

class MainWindow : public KMainWindow, Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public Q_SLOTS:
//     void showContactDetails(QModelIndex index);

private Q_SLOTS:
    void onPersonModelReady();
    void onSelectedContactsChanged(const QItemSelection &selected, const QItemSelection &deselected);
    void onMergeButtonPressed();
    void positionBusyOverlay();

private:
    PersonsDelegate      *m_personsDelegate;
    KPeople::PersonsModel *m_personsModel;
    PersonsProxyModel    *m_personsProxyModel;
    QHash<QUrl, KPeople::PersonDetailsView*> m_cachedDetails;
    KPixmapSequenceWidget *m_busyWidget;
};

#endif // MAIN_WINDOW_H

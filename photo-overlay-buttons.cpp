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


#include "photo-overlay-buttons.h"

#include <QHBoxLayout>
#include <QPushButton>
#include <QSignalMapper>

#include <KIconLoader>
#include <KLocale>


PhotoOverlayButtons::PhotoOverlayButtons(QWidget* parent)
    : QWidget(parent),
      m_signalMapper(0)
{
    m_signalMapper = new QSignalMapper(this);

    QHBoxLayout *layout = new QHBoxLayout(this);

    QPushButton *changePhoto = new QPushButton(this);
    changePhoto->setToolTip(i18nc("@info:tooltip", "Change photo..."));
    changePhoto->setFlat(true);
    changePhoto->setMinimumSize(16, 16);
    m_signalMapper->setMapping(changePhoto, "change-photo");
    connect(changePhoto, SIGNAL(clicked(bool)),
            m_signalMapper, SLOT(map()));

    QPushButton *removePhoto = new QPushButton(this);
    removePhoto->setToolTip(i18nc("@info:tooltip", "Remove photo"));
    removePhoto->setFlat(true);
    removePhoto->setMinimumSize(16, 16);
    m_signalMapper->setMapping(removePhoto, "remove-photo");
    connect(removePhoto, SIGNAL(clicked(bool)),
            m_signalMapper, SLOT(map()));

    layout->setSpacing(0);
    layout->setMargin(0);
    layout->addWidget(changePhoto);
    layout->addWidget(removePhoto);
    setLayout(layout);

    connect(m_signalMapper, SIGNAL(mapped(QString)),
            this, SIGNAL(buttonsClicked(QString)));
}

PhotoOverlayButtons::~PhotoOverlayButtons()
{

}

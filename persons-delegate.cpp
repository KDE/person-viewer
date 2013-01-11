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

#include <QApplication>
#include <QPainter>
#include <QStyle>
#include <QUrl>
#include <kpeople/persons-model.h>
#include <KDebug>
#include <KIconLoader>
#include <KStandardDirs>

#include "persons-delegate.h"

const int SPACING = 8;
const int PHOTO_SIZE = 32;

PersonsDelegate::PersonsDelegate(QObject* parent)
    : QStyledItemDelegate(parent)
{
    m_avatarImagePath = KStandardDirs::locate("data", "person-viewer/dummy_avatar.png");
}

PersonsDelegate::~PersonsDelegate()
{

}

void PersonsDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItemV4 optV4 = option;
    initStyleOption(&optV4, index);

    painter->save();

    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::HighQualityAntialiasing);
    painter->setClipRect(optV4.rect);

    QStyle *style = QApplication::style();
    style->drawPrimitive(QStyle::PE_PanelItemViewItem, &option, painter);

    QRect contactPhotoRect = optV4.rect;
    contactPhotoRect.adjust(SPACING, SPACING, SPACING, SPACING);
    contactPhotoRect.setWidth(PHOTO_SIZE);
    contactPhotoRect.setHeight(PHOTO_SIZE);

    QImage avatar;
    QString avatarPath = index.data(PersonsModel::PhotoRole).toUrl().toLocalFile();

    if (avatarPath.isEmpty()) {
        avatar.load(m_avatarImagePath);
    } else {
        avatar.load(avatarPath);
    }
    painter->drawImage(contactPhotoRect, avatar);

    painter->drawRect(contactPhotoRect);

    QRect nameRect = optV4.rect;
    nameRect.adjust(SPACING + PHOTO_SIZE + SPACING, SPACING, 0, 0);

    painter->drawText(nameRect, index.data(Qt::DisplayRole).toString());

    QRect mailRect = nameRect;
    mailRect.adjust(0, nameRect.height(), 0, 0);

    painter->drawText(mailRect, index.data(PersonsModel::EmailRole).toString());

    painter->restore();
}

QSize PersonsDelegate::sizeHint (const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    return QSize(128, 48);
}

/***************************************************************************
 *   Copyright (C) 2008 by Peter Penz <peter.penz19@gmail.com>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA            *
 ***************************************************************************/

#include "photo-overlay-event-filter.h"

#include <KGlobalSettings>
#include <KIcon>
#include <KIconLoader>
#include <KIconEffect>
#include <KLocale>

#include <QApplication>
#include <QPainter>
#include <QPaintEvent>
#include <QRect>
#include <QTimer>
#include <QTimeLine>

#include <KDebug>

PhotoOverlayEventFilter::PhotoOverlayEventFilter(QWidget* parent)
    : PhotoOverlayButtons(parent),
    m_isHovered(false),
    m_fadingValue(0),
    m_margin(0),
    m_icon(),
    m_fadingTimeLine(0)
{
    setFocusPolicy(Qt::NoFocus);
    parent->installEventFilter(this);
    resize(sizeHint());
    setVisible(false);

    connect(KGlobalSettings::self(), SIGNAL(iconChanged(int)),
            this, SLOT(refreshIcon()));

}

PhotoOverlayEventFilter::~PhotoOverlayEventFilter()
{
}

QSize PhotoOverlayEventFilter::sizeHint() const
{
    return QSize(32, 16);
}

void PhotoOverlayEventFilter::reset()
{
    hide();
}

void PhotoOverlayEventFilter::setMargin(int margin)
{
    if (margin != m_margin) {
        m_margin = margin;
        update();
    }
}

int PhotoOverlayEventFilter::margin() const
{
    return m_margin;
}

void PhotoOverlayEventFilter::setVisible(bool visible)
{
    PhotoOverlayButtons::setVisible(visible);

    stopFading();
    if (visible) {
        //FIXME: fading currently crashes, no idea why
        //startFading();
        m_fadingValue = 255;
    }
}

bool PhotoOverlayEventFilter::eventFilter(QObject* obj, QEvent* event)
{
    //kDebug() << event;
    if ((obj == parent()) && (event->type() == QEvent::Enter)) {
        setVisible(true);
        return true;
    }
    if ((obj == parent()) && (event->type() == QEvent::Leave)) {
        setVisible(false);
        return true;
    }

    return PhotoOverlayButtons::eventFilter(obj, event);
}

void PhotoOverlayEventFilter::enterEvent(QEvent* event)
{
    PhotoOverlayButtons::enterEvent(event);

    // if the mouse cursor is above the selection toggle, display
    // it immediately without fading timer
    m_isHovered = true;
    if (m_fadingTimeLine) {
        m_fadingTimeLine->stop();
    }
    m_fadingValue = 255;
    update();
}

void PhotoOverlayEventFilter::leaveEvent(QEvent* event)
{
    PhotoOverlayButtons::leaveEvent(event);

    m_isHovered = false;
    update();
}

// void PhotoOverlayEventFilter::mousePressEvent(QMouseEvent* event)
// {
//     PhotoOverlayButtons::mousePressEvent(event);
//     //m_leftMouseButtonPressed = (event->buttons() & Qt::LeftButton);
// }
//
// void PhotoOverlayEventFilter::mouseReleaseEvent(QMouseEvent* event)
// {
//     PhotoOverlayButtons::mouseReleaseEvent(event);
//     //m_leftMouseButtonPressed = (event->buttons() & Qt::LeftButton);
// }
//
// void PhotoOverlayEventFilter::resizeEvent(QResizeEvent* event)
// {
//     PhotoOverlayButtons::resizeEvent(event);
//     //setIconOverlay(isChecked());
// }

void PhotoOverlayEventFilter::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setClipRect(event->rect());

    QPixmap icon = KIconLoader::global()->loadIcon("document-open",
                                                   KIconLoader::NoGroup,
                                                   16);
    QPixmap icon2 = KIconLoader::global()->loadIcon("edit-delete",
                                                    KIconLoader::NoGroup,
                                                    16);

    // draw the icon overlay
    const QPoint pos(m_margin, m_margin);
    const QPoint pos2(m_margin + 16, m_margin);
    if (m_isHovered) {
        //FIXME: make only one icon active
        KIconEffect *iconEffect = KIconLoader::global()->iconEffect();
        QPixmap activeIcon = iconEffect->apply(icon, KIconLoader::Desktop, KIconLoader::ActiveState);
        QPixmap activeIcon2 = iconEffect->apply(icon2, KIconLoader::Desktop, KIconLoader::ActiveState);
        painter.drawPixmap(pos, activeIcon);
        painter.drawPixmap(pos2, activeIcon2);
    } else {
        if (m_fadingValue < 255) {
            // apply an alpha mask respecting the fading value to the icon

            QPixmap alphaMask(icon.width(), icon.height());
            const QColor color(m_fadingValue, m_fadingValue, m_fadingValue);
            alphaMask.fill(color);
            icon.setAlphaChannel(alphaMask);
            icon2.setAlphaChannel(alphaMask);
            painter.drawPixmap(pos, icon);
            painter.drawPixmap(pos2, icon2);
        } else {
            // no fading is required
            painter.drawPixmap(pos, icon);
            painter.drawPixmap(pos2, icon2);
        }
    }

}

void PhotoOverlayEventFilter::setFadingValue(int value)
{
    m_fadingValue = value;
    if (m_fadingValue >= 255) {
        Q_ASSERT(m_fadingTimeLine);
        m_fadingTimeLine->stop();
    }
    update();
}

void PhotoOverlayEventFilter::refreshIcon()
{
    //setIconOverlay(isChecked());
}

void PhotoOverlayEventFilter::startFading()
{
    Q_ASSERT(!m_fadingTimeLine);

    const bool animate = KGlobalSettings::graphicEffectsLevel() & KGlobalSettings::SimpleAnimationEffects;
    const int duration = animate ? 600 : 1;

    m_fadingTimeLine = new QTimeLine(duration, this);
    connect(m_fadingTimeLine, SIGNAL(frameChanged(int)),
            this, SLOT(setFadingValue(int)));
    m_fadingTimeLine->setFrameRange(0, 255);
    m_fadingTimeLine->start();
    m_fadingValue = 0;
}

void PhotoOverlayEventFilter::stopFading()
{
    if (m_fadingTimeLine) {
        m_fadingTimeLine->stop();
        delete m_fadingTimeLine;
        m_fadingTimeLine = 0;
    }
    m_fadingValue = 0;
}

#include "photo-overlay-event-filter.moc"

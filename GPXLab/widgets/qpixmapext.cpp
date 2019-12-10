/****************************************************************************
 *   Copyright (c) 2014 - 2015 Frederic Bourgeois <bourgeoislab@gmail.com>  *
 *                                                                          *
 *   This program is free software: you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation, either version 3 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   This program is distributed in the hope that it will be useful,        *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU General Public License for more details.                           *
 *                                                                          *
 *   You should have received a copy of the GNU General Public License      *
 *   along with This program. If not, see <http://www.gnu.org/licenses/>.   *
 ****************************************************************************/

#include "qpixmapext.h"

/////////////////////////////////////////////////////////////////////////////

QPixmapExt::QPixmapExt() :
    QPixmap(),
    dx(0),
    dy(0),
    isFromImg(false)
{
}

QPixmapExt::QPixmapExt(int width, int height, Point::Alignment alignment) :
    QPixmap(width, height),
    isFromImg(false)
{
    alignToTopLeft(alignment);
}

QPixmapExt::QPixmapExt(const QString &fileName, Point::Alignment alignment) :
    QPixmap(fileName),
    isFromImg(true)
{
    alignToTopLeft(alignment);
}

int QPixmapExt::getAlignmentX() const
{
    return dx;
}

int QPixmapExt::getAlignmentY() const
{
    return dy;
}

void QPixmapExt::alignToTopLeft(Point::Alignment alignment)
{
    switch (alignment)
    {
    case Point::Middle:
        dx = -width()/2;
        dy = -height()/2;
        break;

    case Point::TopLeft:
        dx = 0;
        dy = 0;
        break;

    case Point::TopRight:
        dx = -width();
        dy = 0;
        break;

    case Point::BottomLeft:
        dx = 0;
        dy = -height();
        break;

    case Point::BottomRight:
        dx = -width();
        dy = -height();
        break;

    case Point::BottomMiddle:
        dx = -width()/2;
        dy = -height();
        break;

    case Point::TopMiddle:
        dx = -width()/2;
        dy = 0;
        break;
    }
}

bool QPixmapExt::isFromImage() const
{
    return isFromImg;
}

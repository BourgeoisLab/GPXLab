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

#include "wimage.h"
#include "ui_wimage.h"
#include <QPainter>
#include <QStyleOption>

WImage::WImage(QWidget *parent) :
    QWidget(parent)
{

}

WImage::~WImage()
{
}

void WImage::paintEvent(QPaintEvent *)
{
   QStyleOption opt;
   opt.init(this);
   QPainter p(this);
   style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

   if(image.isNull())
        return;

    QPainter painter;
    painter.translate(rect().center());
    painter.scale(scaleRatio, scaleRatio);
    painter.translate(-image.rect().center());
    painter.drawPixmap(0, 0, QPixmap::fromImage(image));
}

void WImage::display(const QString &imageName)
{
    if(imageName.isNull())
        return;

    image = QImage(imageName);
    update();
}

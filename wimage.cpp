#include "wimage.h"
#include "ui_wimage.h"
#include <QPainter>
#include <QStyleOption>

WImage::WImage(QWidget *parent) :
    QWidget(parent), scaleRatio(1.0)
{

}

WImage::~WImage()
{

}

void WImage::paintEvent(QPaintEvent *)
{
    QPainter painter;
    painter.begin(this);
    QStyleOption opt;
    opt.init(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);

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
    if(image.height() > image.width()) {
        image = image.scaledToHeight(height(), Qt::SmoothTransformation);
    } else {
        image = image.scaledToWidth(width(), Qt::SmoothTransformation);
    }
    update();
}

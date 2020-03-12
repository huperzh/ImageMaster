#include "widget.h"
#include "ui_widget.h"
#include "opencv2/opencv.hpp"

#include "MVDShapeCpp.h"
#include "MVDImageCpp.h"
#include "CircleFindCpp.h"
#include "MVDCore.h"
#include <QFileDialog>
#include <QDebug>

using namespace std;
using namespace VisionDesigner;
using namespace VisionDesigner::CircleFind;
using namespace cv;

#define CV_BGR2RGB COLOR_BGR2RGB
#define CV_RGB2GRAY COLOR_RGB2GRAY

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}
/*
 * Function: Open and show local image
 *
 */

void Widget::openImage()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("打开图片"),
           "D:\\IndustryMachineVisionImage\\ImageSample\\Circle", tr("Images (*.png *.bmp *.jpg)"));
    qDebug() << fileName;
    ui->wimage->display(fileName);
}

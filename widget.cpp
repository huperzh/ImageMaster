#include "widget.h"
#include "ui_widget.h"
#include "opencv2/opencv.hpp"

#include "MVDShapeCpp.h"
#include "MVDImageCpp.h"
#include "CircleFindCpp.h"
#include "MVDCore.h"

using namespace std;
using namespace VisionDesigner;
using namespace VisionDesigner::CircleFind;
using namespace cv;

#define CV_BGR2RGB COLOR_BGR2RGB
#define CV_RGB2GRAY COLOR_RGB2GRAY

void detectCircle();
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

void test()
{
    Mat mat;
    mat.create(1000, 300, CV_8UC3);
    //使用ptr遍历Mat
    for (int row = 0; row < mat.rows; row++)
    {
        for (int col = 0; col < mat.cols; col++)
        {
            Vec3b *c = mat.ptr<Vec3b>(row, col);
            c->val[0] = 0;		//B
            c->val[1] = 255;	//G
            c->val[2] = 0;		//R
        }
    }
    imshow("mat", mat);
//    Mat img(rect.height, rect.width, CV_8UC3, (void*)pBuffer);
//      Mat GrayImg;
//      cvtColor(img, img, CV_BGR2RGB);
//      cvtColor(img, GrayImg, CV_RGB2GRAY);

//      medianBlur(GrayImg, GrayImg, 3);
//      int thre = HistThreshold(GrayImg);
//      if(thre < 20)
//      {
//          return pos;
//      }
//      threshold(GrayImg, GrayImg, thre, 255, THRESH_BINARY_INV);
//      Canny(GrayImg, GrayImg, 80, 255, 3);
//      //imwrite dump
//      int l1 = 10000;
//      QPointF posEdge(0, 0);
//      bool inRect = false;
//      for(size_t nrow = 0; nrow < GrayImg.rows; nrow++)
//      {
//          for(size_t ncol = 0; ncol < GrayImg.cols; ncol++)
//          {
//              uchar val = GrayImg.at<uchar>(nrow, ncol);
//              if(val == 255)
//              {
//                  QPoint pos1(ncol, nrow);
//                  QPoint pos2(GrayImg.cols / 2, GrayImg.rows / 2);
//                  QLineF line(pos2, pos1);
//                  double length = line.length();
//                  if(length < l1)
//                  {
//                      l1 = length;
//                      posEdge = pos1;
//                      inRect = true;
//                  }
//              }
//          }
//      }
//      int diagonal = edgeLength * 2;
//      if (posEdge.manhattanLength() > diagonal || (inRect == false))
//      {
//          posEdge = pos;
//      }
//      else
//      {
//          posEdge.setX(posEdge.x() + pos.x() - rect.width / 2);
//          posEdge.setY(posEdge.y() + pos.y() - rect.height / 2);
//      }


    //    Mat GrayImg;
    //    Mat img = imread("");
    //    cvtColor(img, img, CV_BGR2RGB);
    //    cvtColor(img, GrayImg, CV_RGB2GRAY);

    //    medianBlur(GrayImg, GrayImg, 3);
    //    int thre = 0;//HistThreshold(GrayImg);
    //    threshold(GrayImg, GrayImg, thre, 255, THRESH_BINARY_INV);
    //    Canny(GrayImg, GrayImg, 80, 255, 3);
}

int BasicGlobalThreshold(int*pg, int start, int end)
{
    //  基本全局阈值法
    int  i, t, t1, t2, k1, k2;
    double u, u1, u2;
    t = 0;
    u = 0;

    for (i = start; i < end; i++)
    {
        t += pg[i];
        u += i * pg[i];
    }

    k2 = static_cast<int> (u / t);                          //  计算此范围灰度的平均值]

    do
    {
        k1 = k2;
        t1 = 0;
        u1 = 0;

        for(i=start; i <= k1; i++)
        {             //  计算低灰度组的累加和
            t1 += pg[i];
            u1 += i * pg[i];
        }

        t2 = t-t1;
        u2 = u-u1;

        if(t1)
            u1 = u1 / t1;                     //  计算低灰度组的平均值

        else
            u1 = 0;

        if( t2 )
            u2 = u2 / t2;                     //  计算高灰度组的平均值

        else
            u2 = 0;

        k2 = (static_cast<int>(u1 + u2) / 2);                 //  得到新的阈值估计值
    }
    while(k1 != k2);                           //  数据未稳定，继续
    //cout<<"The Threshold of this Image in BasicGlobalThreshold is:"<<k1<<endl;
    return(k1);                              //  返回阈值

}

void detectCircle()
{
    Mat mat = imread("E:\\Practice\\PythonCv\\CvImageDemo\\Image\\calibrateCircle.bmp", IMREAD_GRAYSCALE);
    Mat dst;
    GaussianBlur(mat, dst, Size(3, 3), BORDER_DEFAULT);

    int pg[256], thre;
    int i = 0;
    int es = static_cast<int> (dst.elemSize());

    for (i = 0; i < 256; i++)
        pg[i] = 0;

    for (i = 0; i < dst.rows * dst.cols * es; i++)
        pg[dst.data[i]]++;

    thre = BasicGlobalThreshold(pg, 0, 256);
    qDebug("thre = %d", thre);
    threshold(dst, dst, thre, 255, THRESH_BINARY);
    imshow("dst", dst);
    Mat morphMat;
    Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(19, 19));
    morphologyEx(dst, morphMat, MORPH_OPEN, kernel);
    imshow("morpMat", morphMat);


    //cvMorphologyEx(dst,dst,NULL,myModel,CV_MOP_CLOSE,6);
    //cvSaveImage("/mnt/udisk/DCIM/dst1.bmp",dst);

//    CvSeq* contour = NULL;
//    CvSeq* tmp = NULL;
//    double tmparea = 500.0;
//    CvMemStorage* storage = cvCreateMemStorage(0);
//    CvScalar color = cvScalar(255);//CV_RGB(128,0,0);
//    CvContourScanner scanner = NULL;
//    double distl=0;
//    float distance[100000],tmp_dist;
//    int k=0,k_new=0;
//    scanner = cvStartFindContours(dst,storage,sizeof(CvContour),CV_RETR_TREE,CV_CHAIN_APPROX_SIMPLE,cvPoint(0,0));

//    while (contour=cvFindNextContour(scanner))
//    {
//        Rect rect= boundingRect(contour,0);
//        if (fabs(rect.width-rect.height) > 5)
//            continue;

//        if (fabs(cvContourArea(contour))>tmparea)
//        {
//            tmp = contour;
//            tmparea = cvContourArea(contour);
//            qDebug("tmparea = %f", tmparea);
//        }
//    }

//    double centre_r = -1;
//    double centre_x = -1;
//    double centre_y = -1;

//    contour = cvEndFindContours(&scanner);
//    if(tmp != NULL)
//        cvDrawContours(dst,tmp,color,color,0,CV_FILLED,8,cvPoint(0,0));

//    else
//    {
//        centre_r = -1;
//        centre_x = -1;
//        centre_y = -1;

//        return;
//    }

//    cvMorphologyEx(dst,dst,NULL,NULL,CV_MOP_OPEN,1);
//    cvMorphologyEx(dst,dst,NULL,NULL,CV_MOP_OPEN,1);
//    //cvSaveImage("/mnt/udisk/DCIM/dst11.bmp",dst);

//    double M00,X,Y;
//    CvMoments m;
//    if(tmp != NULL)
//    {
//        cvMoments(tmp,&m,0);
//        M00 = cvGetSpatialMoment(&m,0,0);
//        X = cvGetSpatialMoment(&m,1,0)/M00;
//        Y = cvGetSpatialMoment(&m,0,1)/M00;
//        CvPoint2D32f pt=cvPoint2D32f(X,Y);
//    }

//    cvCanny( dst, dst, 128, 255, 3 );
//    //cvSaveImage("/mnt/udisk/DCIM/cvCanny.bmp",dst);

//    for (int i=0;i<dst->height;i++)
//    {
//        uchar* ptr=(uchar*)(dst->imageData+i*dst->widthStep);
//        for (int j=0;j<dst->width;j++)
//        {
//            if(ptr[j] == 255)
//            {
//                distl = sqrt((j-X)*(j-X)+(i-Y)*(i-Y));
//                distance[k]=distl;
//                k++;
//            }
//        }
//    }


//    for ( int i = 0; i < k-1; i++ )
//    {
//        for ( int j = k-2; j >= i; j -- )
//        {
//            if (distance[ j+1 ] < distance[ j ] )
//            {
//                tmp_dist=distance[j+1];
//                distance[j+1]=distance[j];
//                distance[j]=tmp_dist;
//            }
//        }
//    }

//    for (int i=0;i<k/2;i++)
//    {
//        centre_r+=distance[i];
//        k_new++;
//    }

//    qDebug("k_new = %d, tmp = %p", k_new, tmp);

//    if(k_new && tmp != NULL)
//    {
//        centre_r = centre_r / k_new;
//        qDebug("centre_r = %f", centre_r);
//        centre_x = X + IMG_X;
//        centre_y = Y + IMG_Y;
//    }

//    else
//    {
//        centre_r = -1;
//        centre_x = -1;
//        centre_y = -1;
//    }


}

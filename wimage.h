#ifndef WIMAGE_H
#define WIMAGE_H

#include <QWidget>
#include <QPixmap>

class WImage : public QWidget
{
    Q_OBJECT

public:
    explicit WImage(QWidget *parent = nullptr);
    ~WImage();
    void display(const QString& name);

protected:
    void paintEvent(QPaintEvent *event);

private:
    QImage image;
    qreal scaleRatio;
};

#endif // WIMAGE_H

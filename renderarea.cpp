#include "renderarea.h"

RenderArea::RenderArea(QWidget *parent) :
    QWidget(parent),
    mBackgroundColor(0,0,255),
    mShapeColor(255,255,255),
    mShape (Astroid)
{
    on_shape_changed();
}

QSize RenderArea::minimumSizeHint() const
{
    return QSize(100, 100);
}
QSize RenderArea::sizeHint() const
{
    return QSize(400, 200);
}

void RenderArea::on_shape_changed ()
{
    switch (mShape)
    {
    case Astroid:
        mScale = 100;
        mIntervalLength = 2 * M_PI;
        mStepCount = 512;
        break;

    case Cycloid:

        break;

    case HuygensCycloid:

        break;

    case HypoCycloid:

        break;

    default:
        break;
    }

}

//astroid fuction body:
QPointF RenderArea::compute_astroid (float t)
{
    float cos_t = cos(t);
    float sin_t = sin(t);
    float x = cos_t * cos_t * cos_t;
    float y = sin_t * sin_t * sin_t;
    return QPointF (x, y);
}

void RenderArea::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.setBrush(mBackgroundColor);
    painter.setPen(mShapeColor);

    //drawing area
    painter.drawRect(this->rect());

    QPoint center = this->rect().center();

    float step = mIntervalLength / mStepCount;
    for (float t = 0; t < mIntervalLength; t += step)
    {
        QPointF point = compute_astroid (t);

        QPoint pixel;
        pixel.setX(point.x() * mScale + center.x());
        pixel.setY(point.y() * mScale + center.y());

        painter.drawPoint(pixel);

    }
}

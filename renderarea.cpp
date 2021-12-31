#include "renderarea.h"

RenderArea::RenderArea(QWidget *parent) :
    QWidget(parent),
    mBackgroundColor(0,0,255),
    mPen(Qt::white),
    mShape (Astroid)
{
    mPen.setWidth(2);
    on_shape_changed();
}

QSize RenderArea::minimumSizeHint() const
{
    return QSize(400, 400);
}
QSize RenderArea::sizeHint() const
{
    return QSize(400, 400);
}

void RenderArea::on_shape_changed ()
{
    switch (mShape)
    {
    case Astroid:
        mScale = 180;
        mIntervalLength = 2 * M_PI;
        mStepCount = 256;
        break;

    case Cycloid:
        mScale = 10;
        mIntervalLength = 4 * M_PI;
        mStepCount = 128;
        break;

    case HuygensCycloid:
        mScale = 12;
        mIntervalLength = 4 * M_PI;
        mStepCount = 256;
        break;

    case HypoCycloid:
        mScale = 40;
        mIntervalLength = 2 * M_PI;
        mStepCount = 256;
        break;

    case Line:
        mIntervalLength = 2; //not really needed
        mScale = 198; // line in pixels
        mStepCount = 12;
        break;

    case Circle:
        mScale = 150;
        mIntervalLength = 2 * M_PI;
        mStepCount = 128;
        break;

    case Ellipse:
        mScale = 75;
        mIntervalLength = 2 * M_PI;
        mStepCount = 128;
        break;

    case Fancy:
        mScale = 10;
        mIntervalLength = 12 * M_PI;
        mStepCount = 512;
        break;

    case Starfish:
        mScale = 26;
        mIntervalLength = 6 * M_PI;
        mStepCount = 256;
        break;

    case Cloud:
        mScale = 10;
        mIntervalLength = 28 * M_PI;
        mStepCount = 128;
        break;

    default:
        break;
    }

}

QPointF RenderArea::compute (float t)
{
    switch (mShape)
    {
    case Astroid:
        return compute_astroid(t);
        break;

    case Cycloid:
        return compute_cycloid(t);
        break;

    case HuygensCycloid:
        return compute_huygens(t);
        break;

    case HypoCycloid:
        return compute_hypo(t);
        break;

    case Line:
        return compute_line(t);
        break;

    case Circle:
        return compute_circle(t);
        break;

    case Ellipse:
        return compute_ellipse(t);
        break;

    case Fancy:
        return compute_fancy(t);
        break;

    case Starfish:
        return compute_starfish(t);
        break;

    case Cloud:
        return compute_cloud(t);
        break;

    default:
        break;
    }

    return QPointF (0, 0);
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

QPointF RenderArea::compute_cycloid (float t)
{
    return QPointF(
                1.5 * (1 - cos (t)), //x
                1.5 * (t - sin(t)) //y
                );
}

QPointF RenderArea::compute_huygens (float t)
{
    return QPointF(
                4 * (3 * cos (t) - cos (3 * t)), //x
                4 * (3 * sin (t) - sin (3 * t)) //y
                );
}

QPointF RenderArea::compute_hypo (float t)
{
    return QPointF(
               1.5 * (2 * cos (t) + cos (2 * t)), //x
               1.5 * (2 * sin (t) - sin (2 * t)) //y
                );
}

QPointF RenderArea::compute_line (float t)
{
    return QPointF(
                (1-t), //x
                (1-t) //y
                );
}

QPointF RenderArea::compute_circle (float t)
{
    return QPointF(
               cos (t), //x
               sin (t) //y
                );
}

QPointF RenderArea::compute_ellipse (float t)
{
    float a = 2;
    float b = 1.1;
    return QPointF(
               a * cos (t), //x
               b * sin (t) //y
                );
}

QPointF RenderArea::compute_fancy(float t)
{
    float v1 = 11;
    float v2 = 6;
    return QPointF(
               v1 * cos (t) - v2 * cos((v1/v2) * t), //x
               v1 * sin (t) - v2 * sin((v1/v2) * t) //y
                );
}

QPointF RenderArea::compute_starfish(float t)
{
    float R = 5;
    float r = 3;
    float d = 5;
    return QPointF(
               (R - r) * cos (t) + d * cos(((R - r)/r) * t), //x
               (R - r) * sin (t) - d * sin(((R - r)/r) * t) //y
                );
}

QPointF RenderArea::compute_cloud(float t)
{
    float a = 14;
    float b = 1;
    float sign = 1;
    return QPointF(
               (a + b) * cos (t * b / a) - sign * b * cos(((a + b)/a) * t), //x
               (a + b) * sin (t * b / a) - b * sin(((a + b)/a) * t) //y
                );
}

void RenderArea::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.setBrush(mBackgroundColor);
    painter.setPen(mPen);

    //drawing area
    painter.drawRect(this->rect());

    QPoint center = this->rect().center();

    QPointF prevPoint = compute (0);
    QPoint prevPixel ;
    prevPixel.setX(prevPoint.x() * mScale + center.x());
    prevPixel.setY(prevPoint.y() * mScale + center.y());


    float step = mIntervalLength / mStepCount;
    for (float t = 0; t < mIntervalLength; t += step)
    {
        QPointF point = compute (t);

        QPoint pixel;
        pixel.setX(point.x() * mScale + center.x());
        pixel.setY(point.y() * mScale + center.y());

        painter.drawLine(pixel, prevPixel);

        prevPixel = pixel;

    }

    QPointF point = compute (mIntervalLength);

    QPoint pixel;
    pixel.setX(point.x() * mScale + center.x());
    pixel.setY(point.y() * mScale + center.y());

    painter.drawLine(pixel, prevPixel);
}

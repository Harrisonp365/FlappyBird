#include "birditem.h"
#include <QTimer>
#include <QGraphicsScene>

BirdItem::BirdItem(QPixmap pixmap) : wingPosition(WingPosition::Up), wingDirection(0)

{
    setPixmap(pixmap);
    //Timer to make wings flap by switching pixmap
    QTimer * wingTimer = new QTimer(this);
    connect(wingTimer, &QTimer::timeout,[=](){
        updatePixmap();
    });
    wingTimer->start(80);

    //SetUp the ground POS
    groundPosition = scenePos().y() + 290;

    //Falling Animation
    yAnimation = new QPropertyAnimation(this,"y", this);
    yAnimation->setStartValue(scenePos().y());
    yAnimation->setEndValue(groundPosition);
    yAnimation->setEasingCurve(QEasingCurve::InQuad);
    yAnimation->setDuration(1000);


    //Rotation on fall Animation
    rotationAnimation = new QPropertyAnimation(this, "rotation", this);
    setFlag(QGraphicsItem::ItemIsFocusable);
}

qreal BirdItem::y() const
{
    return m_y;
}

qreal BirdItem::rotation() const
{
    return m_rotation;
}

void BirdItem::shootUp()
{
    yAnimation->stop();
    rotationAnimation->stop();

    qreal curPosY = y();
    yAnimation->setStartValue(curPosY);
    yAnimation->setEndValue(curPosY - scene()->sceneRect().height()/8);
    yAnimation->setEasingCurve(QEasingCurve::OutQuad);
    yAnimation->setDuration(285);

    connect(yAnimation, &QPropertyAnimation::finished,[=](){
        fallToGround();
    });

    yAnimation->start();

    rotateTo(-20, 200, QEasingCurve::OutCubic);
}

void BirdItem::startFlying()
{
    yAnimation->start();
    rotateTo(90, 1000, QEasingCurve::InQuad);
}

void BirdItem::freezeInPlace()
{
    yAnimation->stop();
    rotationAnimation->stop();
}

void BirdItem::setY(qreal y)
{
    moveBy(0, y-m_y);
    m_y = y;
}

void BirdItem::setRotation(qreal rotation)
{
    m_rotation = rotation;
    QPointF c = boundingRect().center();
    QTransform t;
    t.translate(c.x(), c.y());
    t.rotate(rotation);
    t.translate(-c.x(), -c.y());
    setTransform(t);
}

void BirdItem::rotateTo(const qreal &end, const int &duration, const QEasingCurve &curve)
{
    rotationAnimation->setStartValue(rotation());
    rotationAnimation->setEndValue(end);
    rotationAnimation->setEasingCurve(curve);
    rotationAnimation->setDuration(duration);

    rotationAnimation->start();
}

void BirdItem::fallToGround()
{   //Reset animations after shooting Up method
    if(y() < groundPosition)
    {
        rotationAnimation->stop();


        yAnimation->setStartValue(y());
        yAnimation->setEasingCurve(QEasingCurve::InQuad);
        yAnimation->setEndValue(groundPosition);
        yAnimation->setDuration(1000);
        yAnimation->start();

        rotateTo(90, 1100, QEasingCurve::InCubic);
    }
}

void BirdItem::updatePixmap()
{
    if(wingPosition == WingPosition::Middle)
    {
        if(wingDirection)
        {
            //Up
            setPixmap(QPixmap(":/images/bird_blue_up.png"));
            wingPosition = WingPosition::Up;
            wingDirection = 0;
        }
        else
        {
            //Down
            setPixmap(QPixmap(":/images/bird_blue_down.png"));
            wingPosition = WingPosition::Down;
            wingDirection = 1;
        }
    }
    else
    {
        setPixmap(QPixmap(":/images/bird_blue_middle.png"));
        wingPosition = WingPosition::Middle;
    }
}

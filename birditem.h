#ifndef BIRDITEM_H
#define BIRDITEM_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QPropertyAnimation>

class BirdItem : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
    Q_PROPERTY(qreal rotation READ rotation WRITE setRotation)
    Q_PROPERTY(qreal y READ y WRITE setY)
public:
    explicit BirdItem(QPixmap pixmap);

    qreal y() const;

    qreal rotation() const;
    void shootUp();
    void startFlying();

    void freezeInPlace();

public slots:
    void setY(qreal y);

    void setRotation(qreal rotation);
    void rotateTo(const qreal &end, const int &duration, const QEasingCurve &curve);
    void fallToGround();

private:
    enum WingPosition{
        Up,
        Middle,
        Down
    };
    void updatePixmap();
    WingPosition wingPosition;
    bool wingDirection;
    qreal m_y;
    qreal m_rotation;
    QPropertyAnimation * yAnimation;
    QPropertyAnimation * rotationAnimation;
    qreal groundPosition;
};

#endif // BIRDITEM_H

#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include <QTimer>
#include "pillaritem.h"
#include "birditem.h"

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Scene(QObject *parent = nullptr);
    void addBird();
    void startGame();

    bool getGameOn() const;
    void setGameOn(bool value);
    void incrimentScore();


    // QGraphicsScene interface
    void setScore(int value);

protected:
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

private:
    void showGameOverGraphics();
    void hideGameOver();
    void setUpPillarTimer();
    void freezeBirdAndPillar();
    void cleanPillars();
    QTimer * pillarTimer;
    BirdItem * birdItem;
    bool gameOn;

    int score;
    int bestScore;

    QGraphicsPixmapItem * gameOverPix;
    QGraphicsTextItem * scoreTextItem;

};

#endif // SCENE_H

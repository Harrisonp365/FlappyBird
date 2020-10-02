#include "scene.h"
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>

Scene::Scene(QObject *parent) : QGraphicsScene(parent), gameOn(false), score(0), bestScore(0)
{
    setUpPillarTimer();
}

void Scene::addBird()
{
    birdItem = new BirdItem(QPixmap(":/images/bird_blue_up.png"));
    addItem(birdItem);
}

void Scene::startGame()
{
    //Bird
    birdItem->startFlying();
    //Pillars
    if(!pillarTimer->isActive())
    {
        cleanPillars();
        setGameOn(true);
        setScore(0);
        hideGameOver();
        pillarTimer->start(1600);
    }
}

void Scene::setUpPillarTimer()
{
    pillarTimer = new QTimer(this);
    connect(pillarTimer, &QTimer::timeout, [=](){
        PillarItem * pillarItem = new PillarItem();
        connect(pillarItem, &PillarItem::collideFail, [=](){
           pillarTimer->stop();
           freezeBirdAndPillar();
           setGameOn(false);
           showGameOverGraphics();
        });

        addItem(pillarItem);
    });
}

void Scene::freezeBirdAndPillar()
{
    birdItem->freezeInPlace();

    QList<QGraphicsItem*> sceneItems = items();
    foreach(QGraphicsItem * item, sceneItems)
    {
        PillarItem * pillar = dynamic_cast<PillarItem *>(item);
        if(pillar)
        {
                pillar->freezeInPlace();
        }
    }
}

void Scene::cleanPillars()
{
    QList<QGraphicsItem*>sceneItems = items();
    foreach(QGraphicsItem * item, sceneItems)
    {
        PillarItem * pillar = dynamic_cast<PillarItem *>(item);
        if(pillar)
        {
            removeItem(pillar);
            delete pillar;
        }
    }
}

void Scene::setScore(int value)
{
    score = value;
}

bool Scene::getGameOn() const
{
    return gameOn;
}

void Scene::setGameOn(bool value)
{
    gameOn = value;
}

void Scene::incrimentScore()
{
    score++;
    if(score > bestScore)
        bestScore = score;
}

void Scene::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Space)
    {
        if(gameOn)
        {
            birdItem->shootUp();
        }
    }
    QGraphicsScene::keyPressEvent(event);
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        if(gameOn)
        {
            birdItem->shootUp();
        }
    }
    QGraphicsScene::mousePressEvent(event);
}

void Scene::showGameOverGraphics()
{
    gameOverPix = new QGraphicsPixmapItem(QPixmap(":/images/game_over_red.png"));
    addItem(gameOverPix);
    gameOverPix->setPos(QPointF(0, 0) - QPointF(gameOverPix->boundingRect().width() / 2, gameOverPix->boundingRect().height() / 2));

    scoreTextItem = new QGraphicsTextItem();
    QString htmlString = "<p> Score : " + QString::number(score) + "<p>" +
            "<p> Best Score : " + QString::number(bestScore) + "<p>";

    QFont mFont("Consolas", 30, QFont::Bold);

    scoreTextItem->setHtml(htmlString);
    scoreTextItem->setFont(mFont);
    scoreTextItem->setDefaultTextColor(Qt::yellow);
    addItem(scoreTextItem);

    scoreTextItem->setPos(QPointF(0, 0) - QPointF(scoreTextItem->boundingRect().width() / 2, - gameOverPix->boundingRect().height() / 2));
}

void Scene::hideGameOver()
{
    if(gameOverPix)
    {
        removeItem(gameOverPix);
        delete gameOverPix;
        gameOverPix = nullptr;
    }
    if(scoreTextItem)
    {
        removeItem(scoreTextItem);
        delete scoreTextItem;
        scoreTextItem = nullptr;
    }
}

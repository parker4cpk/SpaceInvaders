#include "gamedialog.h"
#include "bullet.h"
#include "ship.h"
#include <QDebug>
#include <QKeyEvent>
#include <QPainter>
#include <QPixmap>
#include <QSound>
#include <QTimer>
#include <QWidget>
#include <vector>
#include <iostream>

namespace game {

/*
 * STAGE 3 NOTES
 * This stage uses the observer design pattern.
 * GameDialog is subscribed to a PlayerScoreGui broadcast.
 * This is so that when a player enters a name, the GameDialog can be notified
 * and store the score in the list of high scores.
 */
GameDialog::GameDialog(QWidget* parent)
        : QDialog(parent), bullets(), shipFiringSound(this), gameScore(0) {
    // SET UP GAME DIMENSIONS AND CONFIG
    Config* c = Config::getInstance();
    SCALEDWIDTH = c->get_SCALEDWIDTH();
    SCALEDHEIGHT = c->get_SCALEDHEIGHT();
    this->frames = c->get_frames();

    levelGenerator = new StandardLevelGenerator(SCALEDWIDTH, SCALEDHEIGHT);

    // MENU
    QList<QPair<QString, int>> dummy;
    menu = new Menu(this, this->gameScore, dummy);

    // EXTENSION STAGE 1 PART 1 - RESCALE GAME SCREEN FOR SHIP SIZE
    this->setFixedWidth(SCALEDWIDTH);
    this->setFixedHeight(SCALEDHEIGHT);
    // SHIP
    QPixmap ship;
    ship.load(":/Images/ship.png");
    this->ship = new Ship(ship, c->get_scale(), c->get_startpos(), SCALEDHEIGHT);
    //this->next_instruct = 0;
    this->leftPressed = false;
    this->rightPressed = false;
    this->lastInstruction = Qt::Key_0;
    this->isShooting = false;
    // SHIP SOUND
    shipFiringSound.setSource(QUrl::fromLocalFile(":/Sounds/shoot.wav"));
    shipFiringSound.setVolume(0.3f);

    // ALIENS
    //generateAliens(c->getSwarmList());
    currentLevelNumber = 1;
    currentLevel = levelGenerator->getLevel(currentLevelNumber);
    generateAliens(*currentLevel);

    // SET BACKGROUND
    setStyleSheet("background-color: #000000;");

    paused = false;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(nextFrame()));
    timer->start(this->frames);

    update();

    livesLeft = 1;
    showScore();

    gameEnded = false;

}

GameDialog::~GameDialog() {
    delete ship;
    delete timer;  // optional, don't have to do this apparently

    delete currentLevel;
    delete levelGenerator;

    // loop though swarms to delete aliens
    delete swarms;  // recursively deletes itself.
    for (Bullet* b : bullets) {
        delete b;
    }

    delete menu;

    delete playerScoreGui;
    delete highScoreGui;

}

// make the swarms for this level.
void GameDialog::generateAliens(const QList<SwarmInfo>& makeSwarms) {
    SwarmInfo def = SwarmInfo();

    swarms = new Swarm(def, *this->ship);
    for (SwarmInfo info : makeSwarms) {
        AlienBase* s = new Swarm(info, *this->ship);
        swarms->add(s);
    }
}

void GameDialog::pauseStart() {
    if (this->paused) {
        // start game
        this->paused = false;
        this->menu->displayMenu(paused);
        this->timer->start(frames);
    } else {
        this->paused = true;
        this->menu->displayMenu(paused);
        this->timer->stop();
    }
}

void GameDialog::keyPressEvent(QKeyEvent* event) {

    int key = event->key();

    switch (key) {

    case Qt::Key_P:
        pauseStart();
        break;

    case Qt::Key_Left:
        leftPressed = true;
        lastInstruction = key;
        break;
    case Qt::Key_Right:
        rightPressed = true;
        lastInstruction = key;
        break;

    case Qt::Key_Space:
        isShooting = true;
        break;

    }

}

void GameDialog::keyReleaseEvent(QKeyEvent* event) {

    switch (event->key()) {

    case Qt::Key_Space:
        isShooting = false;
        break;

    case Qt::Key_Left:
        leftPressed = false;
        break;
    case Qt::Key_Right:
        rightPressed = false;
        break;

    }

}

// shows this game
void GameDialog::showScore() {
    // in future, implement 'score list' in menu.
    menu->openScore();
}

// FOLLOWING EACH INSTRUCTION TO FRAME - for PLAYER ship.
void GameDialog::nextFrame() {

    if (gameEnded)
        return;

    if (!paused) {

        updateShip();
        updateBullets();

        // loop through each alien swarm, move and calculated collisions
        swarms->move("");  // recursive.
        checkSwarmCollisions(swarms);
        addBullets(swarms->shoot(""));

        menu->update();

        if (livesLeft < 1) {
            endGame();
        }

    }
    // prepare collisions and calculate score
    update();

    if (swarms->getAliens().isEmpty()) {
        incrementLevel();
    }

}

void GameDialog::updateShip() {

    /*
    Config* c = Config::getInstance();

    QStringList instruct = c->get_instructs();
    if (next_instruct >= instruct.size()) {
        next_instruct = next_instruct % instruct.size();
    }
    QString ins = instruct[next_instruct];
    next_instruct++;
    */

    int nextInstruction;
    if (leftPressed && rightPressed) {
        nextInstruction = lastInstruction;
    } else if (leftPressed) {
        nextInstruction = Qt::Key_Left;
    } else if (rightPressed) {
        nextInstruction = Qt::Key_Right;
    } else {
        nextInstruction = Qt::Key_0;
    }

    if (nextInstruction == Qt::Key_Left) {
        ship->move_left();
    } else if (nextInstruction == Qt::Key_Right) {
        ship->move_right();
    }

    if (isShooting) {
        bullets.push_back(this->ship->shoot());
        this->shipFiringSound.play();
    }

}

void GameDialog::incrementLevel() {

    delete swarms;

    int numberOfBullets = bullets.size();
    for (int i = 0; i < numberOfBullets; i++) {
        Bullet *b = bullets.back();
        delete b;
        bullets.pop_back();
    }

    delete currentLevel;

    currentLevelNumber++;
    if (currentLevelNumber > 10) {
        endGame();
     }
    currentLevel = levelGenerator->getLevel(currentLevelNumber);
    generateAliens(*currentLevel);

}

void GameDialog::paintBullets(QPainter& painter) {
    for (int i = 0; i < bullets.size(); i++) {
        Bullet* b = bullets[i];
        painter.drawPixmap(b->get_x(), b->get_y(), b->get_image());
    }
}

void GameDialog::updateBullets()
{
    for (int i = 0; i < bullets.size(); i++) {
        Bullet* b = bullets[i];
        // WHEN BULLET OFF GAME SCREEN, FREE MEMORY AND DELETE
        int score = get_collided(b, swarms);
        if (b->get_y() < 0 || b->get_y() >= SCALEDHEIGHT || b->get_x() < 0 ||
                b->get_x() >= SCALEDWIDTH || score != 0) {
            delete b;
            bullets.erase(bullets.begin() + i);
            i--;
//        } else if (score == -1) {
            // DEAD SHIP!
//            close();
        } else
        {
            b->move();// we move at the end so that we can see collisions before the game ends
        }
        gameScore += score;


    }
}

// recurse throughout the tree and draw everything.
// will also
void GameDialog::paintSwarm(QPainter& painter, AlienBase*& root) {
    for (AlienBase* child : root->getAliens()) {
        // if the child is a leaf (i.e., an alien that has no children), draw it.
        const QList<AlienBase*>& list = child->getAliens();
        if (list.size() == 0) {  // leaf
            painter.drawPixmap(child->get_x(), child->get_y(), child->get_image());
        } else {
            paintSwarm(painter, child);
        }
    }
}

//check if any aliens based off the alien root are crashing with the player ship.
void GameDialog::checkSwarmCollisions(AlienBase *&root)
{
    for (AlienBase* child : root->getAliens()) {
        // if the child is a leaf (i.e., an alien that has no children), check for collisions.
        const QList<AlienBase*>& list = child->getAliens();
        if (list.size() == 0) {  // leaf
            // check if it is crashing into the player ship
            if (child->collides(*this->ship)) {
                endGame();  // DEAD SHIP AGAIN
            }
        } else {
          checkSwarmCollisions(child);
        }
    }
}

// PAINTING THE SHIP AND ANY BULLETS
void GameDialog::paintEvent(QPaintEvent*) {

    if (gameEnded) {
        return;
    }

    // SHIP
    QPainter painter(this);

    painter.drawPixmap(ship->get_x(), ship->get_y(), ship->get_image());

    // loop through each alien swarm and draw
    if (swarms != nullptr) {
        paintSwarm(painter, swarms);
    }

    // BULLETS last so they draw over everything
    paintBullets(painter);
}

// if this bullet is unfriendly, only check if it hits Ship
// if this bullet is friendly, will check the swarm;
// returns the score from the deleted hit object.
// Returns 0 if nothing hit, -ve if ship is hit.
int GameDialog::get_collided(Bullet*& b, AlienBase*& root) {
    int totalScore = 0;

    // if it's an enemy bullet, then don't look at the swarm.
    if (!b->isFriendly()) {
        // check it hits the player ship
        if (b->collides(*this->ship)) {
            livesLeft--;
            totalScore = -1;
        }  // future; add barriers here.
    } else {
        totalScore += get_collided_swarm(b, root);
    }
    return totalScore;
}

// helper function, recursively searches swarms.
int GameDialog::get_collided_swarm(Bullet*& b, AlienBase*& root) {
    int totalScore = 0;
    // Case 1: you are a leaf node, or root node
    if (root->getAliens().size() == 0) {
        // check collision.
        if (b->collides(*root)) {
            // if it is an alien, it will return >0.
            // if it's a swarm (i.e., root) it will return 0.
            return root->get_score();
        }

    } else {
        for (int i = 0; i < root->getAliens().size(); i++) {
            AlienBase* child = root->getAliens().at(i);
            // if it's another swarm, recurse down the tree
            totalScore += get_collided_swarm(b, child);
            // if something was hit, score > 0
            if (totalScore > 0 && child->getAliens().size() == 0) {
                // some children shoot more bullets when they die.
                // ask child for reaction when you're going to delete them
                totalScore += child->get_score();
                addBullets(child->react());
                root->remove(child);
                i--;
                return totalScore;
            }
        }
    }
    return totalScore;
}

void GameDialog::addBullets(const QList<Bullet*>& list) {
    for (Bullet* b : list) {
        this->bullets.push_back(b);
    }
}

void GameDialog::endGame() {

    gameEnded = true;
    showScore();
    this->setStyleSheet("background-color:gray");
    playerScoreGui = new PlayerScoreGui(this, gameScore);
    connect(playerScoreGui, SIGNAL(submitHighScore(QString)), this, SLOT(submitHighScore(QString)));
    playerScoreGui->setVisible(true);

}

void GameDialog::submitHighScore(QString name) {

    highScoreGui = new HighScoreGui(this, name, gameScore);
    highScoreGui->setVisible(true);

}

}

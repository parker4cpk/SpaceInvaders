#include "menu.h"

namespace game {
Menu::Menu(QWidget* parent, int& playerScore, QList<QPair<QString, int>> scores)
        : gameScore(playerScore) {
    // Scores could be read in the future.
    makeButtons(parent);
}

Menu::~Menu() {
    delete score;
    delete scoreLabel;
    delete scoreValueLabel;
}

void Menu::makeButtons(QWidget* parent) {
    score = new QPushButton("Score", parent);
    score->setGeometry(QRect(0, 0, 100, 30));
    score->setVisible(false);
    score->setStyleSheet("background-color: red");
    QObject::connect(score, SIGNAL(released()), parent, SLOT(showScore()));

    scoreLabel = new QLabel(parent);
    scoreLabel->setGeometry(0, 30, 100, 30);
    scoreLabel->setText("Score:");
    scoreLabel->setVisible(false);
    scoreLabel->setStyleSheet("background-color: gray");

    scoreValueLabel = new QLabel(parent);
    scoreValueLabel->setGeometry(100, 30, 100, 30);
    scoreValueLabel->setText(QString::number(gameScore));
    scoreValueLabel->setVisible(false);
    scoreValueLabel->setStyleSheet("background-color: gray");
}

// called when game is paused or unpaused
void Menu::displayMenu(bool paused) {
    if (!paused) {
        closeButtons();
    } else {
        score->setVisible(true);
    }
}

// helper, closes all the opened menus
void Menu::closeButtons() {
    score->setVisible(false);
    revealPlayerScore(false);
}

// helper function for OpenScore
void Menu::revealPlayerScore(bool open) {
    // recalculate playerScoreLabel
    update();
    scoreLabel->setVisible(open);
    scoreValueLabel->setVisible(open);
}

void Menu::openScore() {
    if (scoreLabel->isVisible()) {
        revealPlayerScore(false);
    } else {
        revealPlayerScore(true);
    }
}

void Menu::update() {
    scoreValueLabel->setText(QString::number(gameScore));
}

}

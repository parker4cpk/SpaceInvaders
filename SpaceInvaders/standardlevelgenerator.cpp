#include "standardlevelgenerator.h"

namespace game {

/*
 * Generates 10 levels.
 */
StandardLevelGenerator::StandardLevelGenerator(int windowWidth, int windowHeight)
    : m_maxX(windowWidth), m_maxY(windowHeight/2) {}

const QList<SwarmInfo>* StandardLevelGenerator::getLevel(int level) {

    QList<SwarmInfo> *swarmInfos = new QList<SwarmInfo>();

    // Blue aliens
    QList<QPair<int, int>> blueAlienPositions;
    for (int i = 0; i < level; i++) {
        blueAlienPositions.push_back(QPair<int, int>(rand() % m_maxX, rand() % m_maxY));
    }
    QStringList blueAlienMoves;
    for (int i = 0; i < 50; i++) {
        blueAlienMoves.push_back(getRandomMove());
    }
    swarmInfos->push_back(SwarmInfo("blue", blueAlienPositions, blueAlienMoves, 10 - level));

    // Red aliens
    QList<QPair<int, int>> redAlienPositions;
    for (int i = 0; i < level; i++) {
        redAlienPositions.push_back(QPair<int, int>(rand() % m_maxX, rand() % m_maxY));
    }
    QStringList redAlienMoves;
    for (int i = 0; i < 50; i++) {
        redAlienMoves.push_back(getRandomMove());
    }
    swarmInfos->push_back(SwarmInfo("red", redAlienPositions, redAlienMoves, 10 - level));

    // Dumb aliens
    QList<QPair<int, int>> dumbAlienPositions;
    for (int i = 0; i < level; i++) {
        dumbAlienPositions.push_back(QPair<int, int>(rand() % m_maxX, rand() % m_maxY));
    }
    QStringList dumbAlienMoves;
    for (int i = 0; i < 50; i++) {
        dumbAlienMoves.push_back(getRandomMove());
    }
    swarmInfos->push_back(SwarmInfo("dumb", dumbAlienPositions, dumbAlienMoves, 10 - level));

    // Hunter aliens
    QList<QPair<int, int>> hunterAlienPositions;
    for (int i = 0; i < level; i++) {
        hunterAlienPositions.push_back(QPair<int, int>(rand() % m_maxX, rand() % m_maxY));
    }
    QStringList hunterAlienMoves;
    for (int i = 0; i < 50; i++) {
        hunterAlienMoves.push_back(getRandomMove());
    }
    swarmInfos->push_back(SwarmInfo("hunter", hunterAlienPositions, hunterAlienMoves, 10 - level));

    return swarmInfos;

}

const QString StandardLevelGenerator::getRandomMove() {

    int selection = rand() % 4;
    switch(selection) {
    case 0:
        return QString("L");
    case 1:
        return QString("R");
    case 2:
        return QString("U");
    default:
        return QString("D");
    }

}

}

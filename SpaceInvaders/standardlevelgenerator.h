#ifndef STANDARDLEVELGENERATOR_H
#define STANDARDLEVELGENERATOR_H

#include "levelgenerator.h"

namespace game{

class StandardLevelGenerator : public LevelGenerator {
public:
    StandardLevelGenerator(int windowWidth, int windowHeight);
    const QList<SwarmInfo>* getLevel(int level);
private:
    const QString getRandomMove();
    int m_maxX;
    int m_maxY;
};

}

#endif // STANDARDLEVELGENERATOR_H

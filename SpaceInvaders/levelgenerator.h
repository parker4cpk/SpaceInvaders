#ifndef LEVELGENERATOR_H
#define LEVELGENERATOR_H

#include <QList>

#include "swarminfo.h"

namespace game {
class LevelGenerator {
public:
    virtual const QList<SwarmInfo>* getLevel(int level) = 0;
};
}

#endif // LEVELGENERATOR_H

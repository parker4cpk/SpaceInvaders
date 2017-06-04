#ifndef HIGHSCOREGUI_H
#define HIGHSCOREGUI_H

#include <QWidget>

namespace game {

class HighScoreGui {
public:
    HighScoreGui(QWidget *parent, QString name, int score);
    virtual ~HighScoreGui();
private:

};

}

#endif // HIGHSCOREGUI_H

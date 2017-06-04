#ifndef PLAYERSCOREGUI_H
#define PLAYERSCOREGUI_H

#include <qlabel.h>
#include <qlineedit.h>

class PlayerScoreGui
{
public:
    PlayerScoreGui(QWidget *parent, int score);
private:
    QLabel playerScore;
    QLabel nameLabel;
    QLineEdit nameLineEdit;
    int m_score;
};

#endif // PLAYERSCOREGUI_H

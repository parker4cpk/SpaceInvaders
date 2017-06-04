#include "playerscoregui.h"

PlayerScoreGui::PlayerScoreGui(QWidget *parent, int score)
    : m_score(score),
      playerScore(parent),
      nameLabel(parent),
      nameLineEdit(parent) {

}

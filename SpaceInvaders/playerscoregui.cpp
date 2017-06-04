#include "playerscoregui.h"

namespace game {

PlayerScoreGui::PlayerScoreGui(QWidget *parent, int score)
    : m_score(score) {

    // WIDTH = 800
    // HEIGHT = 600

    QFont font;
    font.setPointSize(15);
    font.setBold(true);

    playerScore = new QLabel(parent);
    playerScore->setGeometry(200, 200, 400, 50);
    playerScore->setAlignment(Qt::AlignHCenter);
    playerScore->setAlignment(Qt::AlignCenter);
    playerScore->setFont(font);
    playerScore->setText(QString("Your score was ") + QString::number(score) + QString("."));
    playerScore->setStyleSheet("background-color: gray");
    playerScore->setVisible(false);

    nameLabel = new QLabel(parent);
    nameLabel->setGeometry(200, 300, 200, 50);
    nameLabel->setAlignment(Qt::AlignRight);
    nameLabel->setAlignment(Qt::AlignCenter);
    nameLabel->setFont(font);
    nameLabel->setText(QString("Your name: "));
    nameLabel->setStyleSheet("background-color: gray");
    nameLabel->setVisible(false);

    nameLineEdit = new QLineEdit(parent);
    nameLineEdit->setGeometry(400, 300, 200, 50);
    nameLineEdit->setFont(font);
    nameLineEdit->setStyleSheet("background-color: gray");
    nameLineEdit->setVisible(false);

    submitButton = new QPushButton(QString("Enter high scores."), parent);
    submitButton->setFont(font);
    submitButton->setGeometry(QRect(400, 400, 200, 50));
    submitButton->setVisible(false);

    connect(submitButton, SIGNAL(released()), this, SLOT(submitHighScore()));

}

PlayerScoreGui::~PlayerScoreGui() {
    delete playerScore;
    delete nameLabel;
    delete nameLineEdit;
    delete submitButton;
}

void PlayerScoreGui::setVisible(bool b) {
    playerScore->setVisible(b);
    nameLabel->setVisible(b);
    nameLineEdit->setVisible(b);
    submitButton->setVisible(b);
}

void PlayerScoreGui::submitHighScore() {
    QString name = nameLineEdit->text();
    setVisible(false);
    emit submitHighScore(name);
}

}

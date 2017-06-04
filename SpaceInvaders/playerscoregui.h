#ifndef PLAYERSCOREGUI_H
#define PLAYERSCOREGUI_H

#include <QObject>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

namespace game {

class PlayerScoreGui : public QObject {

    Q_OBJECT

public:
    PlayerScoreGui(QWidget *parent, int score);
    virtual ~PlayerScoreGui();
    void setVisible(bool b);
private:
    int m_score;
    QLabel *playerScore;
    QLabel *nameLabel;
    QLineEdit *nameLineEdit;
    QPushButton *submitButton;
private slots:
    void submitHighScore();
signals:
    void submitHighScore(QString name);
};

}

#endif // PLAYERSCOREGUI_H

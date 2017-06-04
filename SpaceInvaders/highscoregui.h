#ifndef HIGHSCOREGUI_H
#define HIGHSCOREGUI_H

#include <vector>
#include <QWidget>
#include <QPair>
#include <QLabel>

namespace game {

class HighScoreGui {
public:
    HighScoreGui(QWidget *parent, QString name, int score);
    virtual ~HighScoreGui();
    void setVisible(bool b);
private:
    std::vector<QPair<QString, int>> *highScores;

    void initHighScores();
    std::vector<QPair<QString, int>>* retrieveStoredHighScores();

    void sortHighScore();
    static bool compareHighScores(QPair<QString, int> first, QPair<QString, int> second);

    void storeHighScores();

    QLabel *display;
    void setupGui(QWidget *parent);
};

}

#endif // HIGHSCOREGUI_H

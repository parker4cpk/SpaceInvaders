#ifndef MENU_H
#define MENU_H

#include <QLabel>
#include <QPushButton>
namespace game {
class Menu {
public:
    Menu(QWidget* parent, int& playeScore, QList<QPair<QString, int>> scores);
    ~Menu();
    void displayMenu(bool paused);
    void openScore();
    void update();

private:
    void makeButtons(QWidget* parent);
    void closeButtons();  // if any buttons are left open, close them
    int& gameScore;
    QPushButton* score;

    QLabel* scoreLabel;
    QLabel* scoreValueLabel;
    void revealPlayerScore(bool open);
};
}
#endif  // SCOREBUTTON_H

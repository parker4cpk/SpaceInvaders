#include "highscoregui.h"

#include <iterator>
#include <QFile>
#include <QTextStream>
#include <iostream>

namespace game {

HighScoreGui::HighScoreGui(QWidget *parent, QString name, int score) {

    initHighScores();

    QPair<QString, int> highScore(name, score);
    highScores->push_back(highScore);

    std::sort(highScores->begin(), highScores->end(), compareHighScores);

    storeHighScores();

    setupGui(parent);

}

HighScoreGui::~HighScoreGui() {
    delete highScores;
    delete display;
}

void HighScoreGui::initHighScores() {

    std::vector<QPair<QString, int>> *storedHighScores = retrieveStoredHighScores();

    if (storedHighScores == nullptr) {
        highScores = new std::vector<QPair<QString, int>>();
    } else {
        highScores = storedHighScores;
    }

}

std::vector<QPair<QString, int>>* HighScoreGui::retrieveStoredHighScores() {

    QFile highScoreFile("../SpaceInvaders/highscores.txt");

    if (!highScoreFile.open(QIODevice::ReadOnly)) {
        std::cout << "1" << std::endl;
        highScoreFile.close();
        return nullptr;
    }

    std::vector<QPair<QString, int>> *storedHighScores = new std::vector<QPair<QString, int>>();

    QTextStream in(&highScoreFile);
    while (!in.atEnd()) {

        QString line = in.readLine();
        QStringList tokens = line.split(QString("\t"));

        if (tokens.length() != 2) {
            std::cout << "2" << std::endl;
            highScoreFile.close();
            delete storedHighScores;
            return nullptr;
        }

        QRegExp re("\\d*");
        if (!re.exactMatch(tokens.at(1))) {
            std::cout << "3" << std::endl;
            highScoreFile.close();
            delete storedHighScores;
            return nullptr;
        }

        QPair<QString, int> highScore(tokens.at(0), tokens.at(1).toInt());
        storedHighScores->push_back(highScore);

    }

    highScoreFile.close();
    return storedHighScores;

}

bool HighScoreGui::compareHighScores(QPair<QString, int> first, QPair<QString, int> second) {
    return first.second > second.second;
}

void HighScoreGui::storeHighScores() {

    QFile file("../SpaceInvaders/highscores.txt");
    if (file.open(QIODevice::WriteOnly)) {

        QTextStream stream(&file);

        for (int i = 0; i < highScores->size(); i++) {
            if (i != 0)
                stream << "\n";
            stream << highScores->at(i).first << "\t" << highScores->at(i).second;
        }

    }

    file.close();

}

void HighScoreGui::setupGui(QWidget *parent) {

    QFont font;
    font.setPointSize(15);
    font.setBold(true);

    QString text;
    int i = 0;
    while (i < 5 && i < highScores->size()) {
        QPair<QString, int> highScore = highScores->at(i);
        text = text + QString("\n") + highScore.first + QString("\t") + QString::number(highScore.second);
        i++;
    }

    display = new QLabel(parent);
    display->setGeometry(0, 0, 800, 600);
    display->setAlignment(Qt::AlignHCenter);
    display->setAlignment(Qt::AlignCenter);
    display->setFont(font);
    display->setText(text);
    display->setStyleSheet("background-color: gray");
    display->setVisible(false);

}

void HighScoreGui::setVisible(bool b) {
    display->setVisible(b);
}

}

#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <QKeyEvent>
#include <QLabel>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QMessageBox>
#include <QPushButton>
#include "game.h"
#include "cell.h"

class GameBoard : public QWidget
{
    Q_OBJECT
public:
    explicit GameBoard(QWidget *parent = 0);
    void resetGame();
signals:
    void returnToMainMenu();

private:
    Game game;
    Cell *cells[NCells][NCells]{};
    QVBoxLayout *mainLayout;
    QGridLayout *boardLayout;
    QLabel *score;

    void drawBoard();
    void checkGameStatus();

protected:
    void keyPressEvent(QKeyEvent *event);
    void closeEvent(QCloseEvent *event);
};

#endif // GAMEBOARD_H

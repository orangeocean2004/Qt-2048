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
    explicit GameBoard(const QString &user, QWidget *parent = nullptr);
    void resetGame();
    void setCurrentUser(const QString &user);
signals:
    void returnToMainMenu();

private:
    Game game;
    Cell *cells[NCells][NCells]{};
    QVBoxLayout *mainLayout;
    QGridLayout *boardLayout;
    QLabel *score;
    QLabel *stepLabel;
    QString currentUser;
    int steps;

    void drawBoard();
    void checkGameStatus();

protected:
    void keyPressEvent(QKeyEvent *event);
    void closeEvent(QCloseEvent *event);
};

#endif // GAMEBOARD_H
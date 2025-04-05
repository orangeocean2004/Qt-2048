#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include "gameboard.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void startGame();
    void showLeaderboard();
    void quitGame();

private:
    QWidget *centralWidget;
    QVBoxLayout *mainLayout;

    QPushButton *startGameButton;
    QPushButton *leaderboardButton;
    QPushButton *quitButton;

    QLabel *titleLabel;

    GameBoard *gameBoard;
};

#endif // MAINWINDOW_H

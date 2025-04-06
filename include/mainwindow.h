#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMessageBox>
#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QStackedWidget>
#include "gameboard.h"
#include "database.h"
#include "login.h"

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
    void userLogin();
    void onLoginSuccess(const QString &username);
    void logoutUser();
    void showChangePasswordDialog();

private:
    /* 核心部件 */
    QStackedWidget *stackedWidget;
    QWidget *menuWidget;
    Login *loginWidget;
    GameBoard *gameBoard;

    /* 主菜单部件 */
    QWidget *centralWidget;
    QVBoxLayout *mainLayout;
    QPushButton *startGameButton;
    QPushButton *leaderboardButton;
    QPushButton *quitButton;
    QLabel *titleLabel;
    QPushButton *logoutButton;
    QPushButton *changePasswordButton;

    /* 当前用户 */
    QString currentUser;

    void setupMenuWidget();
    void setupLoginWidget();
    void setupMessageBoxStyle(QMessageBox &msgBox);
};

#endif // MAINWINDOW_H

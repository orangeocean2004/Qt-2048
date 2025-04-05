#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include <QFont>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), gameBoard(nullptr)
{
    setWindowTitle("2048 game");
    resize(650, 650);

    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(20);
    mainLayout->setAlignment(Qt::AlignCenter);

    titleLabel = new QLabel("2048", this);
    QFont titleFont("Arial", 48, QFont::Bold);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("color:#776e65;");

    startGameButton = new QPushButton("start!", this);
    leaderboardButton = new QPushButton("rank.", this);
    quitButton = new QPushButton("quit?", this);

    // 设置按钮样式
    QString buttonStyle = "QPushButton {"
                          "background-color: #8f7a66;"
                          "color: white;"
                          "font-weight: bold;"
                          "border-radius: 10px;"
                          "padding: 15px;"
                          "font-size: 18px;"
                          "min-width: 200px;"
                          "}"
                          "QPushButton:hover {"
                          "background-color: #9f8a76;"
                          "}";

    startGameButton->setStyleSheet(buttonStyle);
    leaderboardButton->setStyleSheet(buttonStyle);
    quitButton->setStyleSheet(buttonStyle);

    connect(startGameButton, &QPushButton::clicked, this, &MainWindow::startGame);
    connect(leaderboardButton, &QPushButton::clicked, this, &MainWindow::showLeaderboard);
    connect(quitButton, &QPushButton::clicked, this, &MainWindow::quitGame);

    mainLayout->addStretch();
    mainLayout->addWidget(titleLabel);
    mainLayout->addSpacing(40);
    mainLayout->addWidget(startGameButton);
    mainLayout->addWidget(leaderboardButton);
    mainLayout->addWidget(quitButton);
    mainLayout->addStretch();

    setStyleSheet("MainWindow { background-color: #faf8ef; }");
}

MainWindow::~MainWindow()
{
    if (gameBoard)
    {
        delete gameBoard;
    }
}

void MainWindow::startGame()
{
    hide();

    if (!gameBoard)
    {
        gameBoard = new GameBoard();

        connect(gameBoard, &GameBoard::returnToMainMenu, this, [=]()
                { show();
                gameBoard->hide(); });
    }

    gameBoard->resetGame();
    gameBoard->show();
}

void MainWindow::showLeaderboard()
{
    /*?????*/
}

void MainWindow::quitGame()
{
    QApplication::quit();
}

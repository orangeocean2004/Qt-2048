#include "gameboard.h"
#include "cell.h"
#include <cstdlib>
#include "database.h"

GameBoard::GameBoard(const QString &user, QWidget *parent)
    : QWidget(parent), currentUser(user)
{
    steps = 0; // 初始化步数
    mainLayout = new QVBoxLayout(this);
    boardLayout = new QGridLayout();
    score = new QLabel("SCORE: 0", this);
    stepLabel = new QLabel("STEPS: 0", this);

    QHBoxLayout *infoLayout = new QHBoxLayout();
    infoLayout->addWidget(stepLabel);
    infoLayout->addStretch();
    infoLayout->addWidget(score);

    mainLayout->addLayout(infoLayout);
    mainLayout->addLayout(boardLayout);

    drawBoard();
}

void GameBoard::drawBoard()
{
    delete boardLayout;
    boardLayout = new QGridLayout();
    for (int i = 0; i < NCells; ++i)
    {
        for (int j = 0; j < NCells; ++j)
        {
            delete cells[i][j];
            cells[i][j] = new Cell(game.board[i][j]);

            // 设置固定宽高比
            cells[i][j]->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
            cells[i][j]->setMinimumSize(140, 140); // 设置最小尺寸
            cells[i][j]->setMaximumSize(140, 140); // 设置最大尺寸

            boardLayout->addWidget(cells[i][j], i, j);
            cells[i][j]->draw();
        }
    }
    mainLayout->insertLayout(0, boardLayout);
}

void GameBoard::keyPressEvent(QKeyEvent *event)
{
    if (game.isGameover())
    {
        return;
    }

    switch (event->key())
    {
    case Qt::Key_Up:
        game.move(false, false);
        break;

    case Qt::Key_Down:
        game.move(false, true);
        break;

    case Qt::Key_Left:
        game.move(true, false);
        break;

    case Qt::Key_Right:
        game.move(true, true);
        break;
    }

    if (game.changed)
    {
        if (!game.isfull())
        {
            auto pos = game.getEmptyPos();
            game.board[pos[0]][pos[1]] = 2;
        }
        score->setText(QString("SCORE: %1").arg(game.total_score));

        // 更新步数
        steps++;
        stepLabel->setText(QString("STEPS: %1").arg(steps));

        drawBoard();
        checkGameStatus();
    }
}

void GameBoard::checkGameStatus()
{
    if (game.isWin() || game.isLose())
    {
        // 保存分数到数据库
        if (!currentUser.isEmpty())
        {
            if (!Database::instance().updateUserScore(currentUser, game.total_score))
            {
                qDebug() << "Failed to save score to database.";
            }
        }

        QMessageBox msgBox(this);
        if (game.isWin())
        {
            msgBox.setWindowTitle(tr("恭喜"));
            msgBox.setText(tr("<div style='font-size:18pt; font-weight:bold; color:#776e65;'>你成功合成了2048!</div>"));
        }
        else
        {
            msgBox.setWindowTitle(tr("游戏结束"));
            msgBox.setText(tr("<div style='font-size:18pt; font-weight:bold; color:#776e65;'>游戏结束!</div>"));
        }

        msgBox.setInformativeText(tr("<div style='font-size:14pt; color:#776e65;'>最终得分: %1</div>").arg(game.total_score));
        msgBox.setStandardButtons(QMessageBox::NoButton);

        QPushButton *newGameBtn = msgBox.addButton(tr("再来一局"), QMessageBox::AcceptRole);
        QPushButton *menuBtn = msgBox.addButton(tr("返回菜单"), QMessageBox::RejectRole);

        QString btnStyle = "QPushButton {"
                           "background-color: #8f7a66;"
                           "border-radius: 6px;"
                           "font-weight: bold;"
                           "color: white;"
                           "padding: 10px 20px;"
                           "min-width: 100px;"
                           "}"
                           "QPushButton:hover {"
                           "background-color: #9f8a76;"
                           "}";
        newGameBtn->setStyleSheet(btnStyle);
        menuBtn->setStyleSheet(btnStyle);

        msgBox.setStyleSheet("QMessageBox {"
                             "background-color: #faf8ef;"
                             "}"
                             "QLabel {"
                             "color: #776e65;"
                             "}");

        msgBox.exec();

        if (msgBox.clickedButton() == newGameBtn)
        {
            resetGame();
        }
        else
        {
            emit returnToMainMenu();
            hide();
        }
    }
}

void GameBoard::resetGame()
{
    game.resetGame();
    steps = 0; // 重置步数
    stepLabel->setText(QString("STEPS: %1").arg(steps));
    drawBoard();
    score->setText(QString("SCORE: %1").arg(game.total_score));
}

void GameBoard::closeEvent(QCloseEvent *event)
{
    // 发出返回主菜单的信号
    emit returnToMainMenu();
    // 允许窗口关闭
    event->accept();
}

void GameBoard::setCurrentUser(const QString &user)
{
    currentUser = user;
}
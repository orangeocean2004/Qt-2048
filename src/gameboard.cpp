#include "gameboard.h"
#include "cell.h"
#include <cstdlib>

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
        drawBoard();
        checkGameStatus();
    }
}

GameBoard::GameBoard(QWidget *parent) : QWidget(parent)
{
    /* 游戏窗口大小 */
    resize(650, 650);

    /* 创建主窗口 */
    mainLayout = new QVBoxLayout();
    setLayout(mainLayout);
    boardLayout = new QGridLayout();

    drawBoard();
    mainLayout->insertLayout(0, boardLayout);

    /* 增加计分板 */
    score = new QLabel(QString("SCORE: %1").arg(0));
    score->setStyleSheet("QLabel {color: rgb(235,224,214);font:16pt;}");
    score->setFixedHeight(50);
    mainLayout->insertWidget(1, score, 0, Qt::AlignRight);

    /* 设置样式表 */
    setStyleSheet("GameBoard {background-color: rgb(187,173,160)}");
}

void GameBoard::checkGameStatus()
{
    if (game.isWin())
    {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle(tr("恭喜"));
        msgBox.setText(tr("<div style='font-size:18pt; font-weight:bold; color:#776e65;'>你成功合成了2048!</div>"));
        msgBox.setInformativeText(tr("<div style='font-size:14pt; color:#776e65;'>最终得分: %1</div>").arg(game.total_score));
        msgBox.setStandardButtons(QMessageBox::NoButton);

        QPushButton *newGameBtn = msgBox.addButton(tr("新游戏"), QMessageBox::AcceptRole);
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
            game.resetGame();
            drawBoard();
            score->setText(QString("SCORE: %1").arg(game.total_score));
        }
        else
        {
            emit returnToMainMenu();
            hide();
        }
    }
    else if (game.isLose())
    {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle(tr("游戏结束"));
        msgBox.setText(tr("<div style='font-size:18pt; font-weight:bold; color:#776e65;'>游戏结束!</div>"));
        msgBox.setInformativeText(tr("<div style='font-size:14pt; color:#776e65;'>无法继续移动!<br>最终得分: %1</div>").arg(game.total_score));
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
            game.resetGame();
            drawBoard();
            score->setText(QString("SCORE: %1").arg(game.total_score));
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
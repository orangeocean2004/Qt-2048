#ifndef GAME_H
#define GAME_H

#include <array>
const int NCells = 4;
using Pos = std::array<int, 2>;

class Game
{
public:
    int board[NCells][NCells];

    int total_score = 0;
    bool changed;

    Game();

    /* 移动步骤 */
    void move(bool horizonal, bool reverse);
    /* 重开 */
    void resetGame();
    /* 寻找空位 */
    Pos getEmptyPos();
    /* 判断满屏 */
    bool isfull();
    /* 判断结束 */
    bool isGameover();
    /* 合成出2048 */
    bool isWin();
    /* 没赢 */
    bool isLose();
};

#endif // GAME_H

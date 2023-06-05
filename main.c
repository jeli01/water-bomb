#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <time.h>

enum ColorType {
    BLACK,  	//0
    darkBLUE,	//1
    DarkGreen,	//2
    darkSkyBlue,    //3
    DarkRed,  	//4
    DarkPurple,	//5
    DarkYellow,	//6
    GRAY,		//7
    DarkGray,	//8
    BLUE,		//9
    GREEN,		//10
    SkyBlue,	//11
    RED,		//12
    PURPLE,		//13
    YELLOW,		//14
    WHITE		//15
} COLOR;
enum Block {
    BlockSolid = 100,
    BlockWeak,
} BLOCK;
enum Item {
    ItemHeart = 200,
    ItemBomb,
    ItemPower
} ITEM;
enum Bomb {
    BombFour = 300,
    BombFour2,
    BombThree,
    BombThree2,
    BombTwo,
    BombTwo2,
    BombOne,
    BombOne2,
    BombZero,
} BOMB;
enum Pc {
    PcNormal = 400,
    PcOnBomb
} PC;
enum Npc {
    NpcNormal = 500
} NPC;

#define KUP 72
#define KDOWN 80
#define KLEFT 75
#define KRIGHT 77
#define KSPACE 32

#define GBOARD_WIDTH 15
#define GBOARD_HEIGHT 15
#define GBOARD_ORIGIN_X 4
#define GBOARD_ORIGIN_Y 2

void SetCurrentCursorPos(int x, int y);
COORD GetCurrentCursorPos(void);
void RemoveCursor(void);
void drawingTotalMap();

int gameBoardInfo[GBOARD_HEIGHT + 2][GBOARD_WIDTH + 2] = {
    {100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100},
    {100,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,100},
    {100,0  ,100,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,100},
    {100,0  ,0  ,0  ,400,0  ,0  ,0  ,0  ,0  ,0  ,200,0  ,0  ,0  ,0  ,100},
    {100,0  ,0  ,0  ,0  ,100,0  ,0  ,0  ,0  ,0  ,0  ,0  ,101,0  ,0  ,100},
    {100,0  ,300,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,100},
    {100,0  ,301,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,100,0  ,0  ,0  ,200,100},
    {100,0  ,302,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,100},
    {100,0  ,303,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,100},
    {100,0  ,304,0  ,0  ,0  ,0  ,100,101,0  ,0  ,0  ,0  ,0  ,0  ,0  ,100},
    {100,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,100,0  ,0  ,100},
    {100,0  ,0  ,100,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,101,0  ,100},
    {100,0  ,0  ,0  ,0  ,0  ,200,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,100},
    {100,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,100},
    {100,0  ,0  ,0  ,0  ,0  ,0  ,101,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,100},
    {100,0  ,0  ,0  ,0  ,0  ,101,0  ,0  ,0  ,200,0  ,0  ,0  ,0  ,0  ,100},
    {100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100},
};

int main() {
    RemoveCursor();

    drawingTotalMap(); 
    return 0;
}

void SetCurrentCursorPos(int x, int y)
{
    COORD position = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
}

COORD GetCurrentCursorPos(void)
{
    COORD curPoint;
    CONSOLE_SCREEN_BUFFER_INFO curInfo;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
    curPoint.X = curInfo.dwCursorPosition.X;
    curPoint.Y = curInfo.dwCursorPosition.Y;
    return curPoint;
}

void RemoveCursor(void)
{
    CONSOLE_CURSOR_INFO curInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
    curInfo.bVisible = 0;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}

void drawingTotalMap() {
    int x, y;
    int cursX, cursY;

    for (y = 0; y < GBOARD_HEIGHT + 2; y++)
    {
        for (x = 0; x < GBOARD_WIDTH + 2; x++)
        {
            cursX = x * 2 + GBOARD_ORIGIN_X;
            cursY = y + GBOARD_ORIGIN_Y;

            SetCurrentCursorPos(cursX, cursY);
            if (0 <= gameBoardInfo[y][x] && gameBoardInfo[y][x] < 100) {
                switch (gameBoardInfo[y][x]) {
                case 0:
                    printf("  ");
                    break;
                default:
                    break;
                }
            }
            else if (100 <= gameBoardInfo[y][x] && gameBoardInfo[y][x] < 200) {
                switch (gameBoardInfo[y][x]) {
                case BlockSolid:
                    printf("■");
                    break;
                case BlockWeak:
                    printf("▦");
                    break;
                default:
                    break;
                }
            }
            else if (200 <= gameBoardInfo[y][x] && gameBoardInfo[y][x] < 300) {
                switch (gameBoardInfo[y][x]) {
                case ItemHeart:
                    printf("♥");
                    break;
                case ItemBomb:
                    printf("＠");
                    break;
                case ItemPower:
                    printf("♨");
                    break;
                default:
                    break;
                }
            }
            else if (300 <= gameBoardInfo[y][x] && gameBoardInfo[y][x] < 400) {
                switch (gameBoardInfo[y][x]) {
                case BombFour:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GREEN);
                    printf("Θ");
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
                    break;
                case BombFour2:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GREEN);
                    printf("ㅇ");
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
                    break;
                case BombThree:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), YELLOW);
                    printf("Θ");
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
                    break;
                case BombThree2:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), YELLOW);
                    printf("ㅇ");
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
                    break;
                case BombTwo:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), RED);
                    printf("Θ");
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
                    break;
                case BombTwo2:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), RED);
                    printf("ㅇ");
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
                    break;
                case BombOne:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DarkRed);
                    printf("Θ");
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
                    break;
                case BombOne2:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DarkRed);
                    printf("ㅇ");
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
                    break;
                case BombZero:
                    printf("※");
                    break;
                default:
                    break;
                }
            }
            else if (400 <= gameBoardInfo[y][x] && gameBoardInfo[y][x] < 500) {
                switch (gameBoardInfo[y][x]) {
                case PcNormal:
                    printf("♀");
                    break;
                case PcOnBomb:
                    printf("㉾");
                    break;
                default:
                    break;
                }
            }
            else if (500 <= gameBoardInfo[y][x] && gameBoardInfo[y][x] < 600) {
                switch (gameBoardInfo[y][x]) {
                case NpcNormal:
                    printf("§");
                    break;
                default:
                    break;
                }
            }
            else {
                printf("넌 이상한 값을 넣었어 디버깅해");
            }
        }
    }
}
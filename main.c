#pragma warning (disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <string.h>
#include <time.h>

typedef enum ColorType {        // 색깔 ENUM으로 정의
    BLACK, //0
    darkBLUE, //1
    DarkGreen, //2
    darkSkyBlue, //3
    DarkRed, //4
    DarkPurple, //5
    DarkYellow, //6
    GRAY, //7
    DarkGray, //8
    BLUE, //9
    GREEN, //10
    SkyBlue, //11
    RED, //12
    PURPLE, //13
    YELLOW, //14
    WHITE //15
}COLOR;
enum Block {                // 게임 내에서 Block 종류를 정의
    BlockSolid = 100,
    BlockWeak = 101,
    BossWeakBlock = 130,
    BossWeakBlockAttacked = 131

}BLOCK;
enum Item {                 // 게임 내에서 Item 종류를 정의
    ItemHeart = 200,
    ItemBomb = 201,
    ItemPower = 202
} ITEM;
enum Bomb {                 // 게임 내에서 Bomb 종류를 정의
    BombFour = 300,
    BombFour2 = 301,
    BombThree = 302,
    BombThree2 = 303,
    BombTwo = 304,
    BombTwo2 = 305,
    BombOne = 306,
    BombOne2 = 307,
    BombZero = 310
} BOMB;
enum Pc {               // 게임내에서 PC의 상태를 정의
    PcNormal = 400,
    PcOnBomb = 401,
    PcOnHit = 402
} PC;
enum Npc {                      // 게임 내에서 Npc의 종류를 정의
    NpcPattern = 500,
    NpcNOPattern = 501
} NPC;

typedef struct PCc {            // PC의 움직임을 정의
    COORD pos;

}PC_pos;
typedef struct NPCc {           // NPC1의 상태를 정의
    COORD pos;
    BOOLEAN live;
}NPC_pos_pattern;
typedef struct NPCcc {          // NPC2의 상태를 정의
    COORD pos;
    BOOLEAN live;
}NPC_pos_nopattern;
typedef struct mainCharacterInfo {          // PC의 상태를 정의
    int hp;
    int bombNum; //캐릭터가 놓은 Bomb의 갯수
    int plusBombNumItem;
    int plusBombPowerItem;

}mainCharacterInfo;
mainCharacterInfo MainCharacter;                
typedef struct bossCharacterInfo                    // 보스의 상태를 정의
{
    int phase;
    int boss_hp;

}bossCharacterInfo;
bossCharacterInfo bossCharacter;



#define KUP 72                                      // 상수들을 정의합니다.
#define KDOWN 80
#define KLEFT 75
#define KRIGHT 77
#define KSPACE 32

#define GBOARD_WIDTH 15
#define GBOARD_HEIGHT 15
#define GBOARD_ORIGIN_X 4
#define GBOARD_ORIGIN_Y 2
#define STATUS_MENU_WINDOW_X 40
#define STATUS_MENU_WINDOW_Y 2
#define BOSS_START_X 5
#define BOSS_START_Y 7

PC_pos* pc;
NPC_pos_pattern* npc_pattern;
NPC_pos_nopattern* npc_nopattern;
int npcspeed = 12;
int cnt_npc_pattern;
int cnt_npc_nopattern;
int curMenu = 0;
int whatClick = 0;
int wantExit = 0;
void drawingTotalMap();
void printHeroHp();
void printGameBoard();
void drawBombNumUI();
void drawPowerUI();
void drawNpcHP();
void settingUiInit();
int nextStage();
void drawingGameMenu();
void menuUp();
void menuDown();
void erazeWindow();

double TimeBoardInfo[GBOARD_HEIGHT + 2][GBOARD_WIDTH + 2];
//플레이어가 물풍선닿았을 때 색깔 바뀌는

int stageNum = 1;
// 게임판을 정의합니다.
int gameBoardInfo[GBOARD_HEIGHT + 2][GBOARD_WIDTH + 2] = {
{100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100},
{100,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,100},
{100,0 ,100,100,100,100,100,100,100,100,100,100,0 ,101,100,0 ,100},
{100,0 ,0 ,0 ,100,0 ,0 ,0 ,100,0 ,100,0 ,0 ,100,0 ,0 ,100},
{100,0 ,100,0 ,100,0 ,100,201,100,0 ,0 ,0 ,100,101,0 ,0 ,100},
{100,0 ,0 ,0 ,100,0 ,100,0 ,100,0 ,100,100,100,100,0 ,0 ,100},
{100,0 ,100,100,100,0 ,0 ,0 ,100,100,100,0 ,100,101,0 ,0 ,100},
{100,0 ,0 ,0 ,100,0 ,100,0 ,0 ,0 ,0 ,0 ,0 ,100,0 ,0 ,100},
{100,101,100,0 ,0 ,0 ,100,0 ,100,100,0 ,100,0 ,0 ,100,0 ,100},
{100,100,100,0 ,100,100,100,0 ,202,0 ,0 ,100,100,0 ,100,0 ,100},
{100,0 ,100,0 ,0 ,0 ,100,0 ,0 ,100,0 ,0 ,100,0 ,0 ,100,100},
{100,0 ,0 ,0 ,100,0 ,0 ,0 ,100,0 ,100,100,100,0 ,0 ,100,100},
{100,0 ,100,0 ,100,100,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,100,100,100},
{100,100,100,0 ,100,101,100,100,101,101,100,100,100,0 ,0 ,100,100},
{100,100,101,0 ,0 ,0 ,0 ,0 ,100,100,0 ,0 ,0 ,0 ,100,100,100},
{100,0 ,0 ,0 ,100,0 ,100,101,0 ,200,0 ,100,0 ,0 ,0 ,100,100},
{100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100},
};

int gameBoardInfo1[GBOARD_HEIGHT + 2][GBOARD_WIDTH + 2] = {
{100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100},
{100,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,100},
{100,0 ,100,100,100,100,100,100,100,100,100,100,0 ,101,100,0 ,100},
{100,0 ,0 ,0 ,100,0 ,0 ,0 ,100,0 ,100,0 ,0 ,100,0 ,0 ,100},
{100,0 ,100,0 ,100,0 ,100,201,100,0 ,0 ,0 ,100,101,0 ,0 ,100},
{100,0 ,0 ,0 ,100,0 ,100,0 ,100,0 ,100,100,100,100,0 ,0 ,100},
{100,0 ,100,100,100,0 ,0 ,0 ,100,100,100,0 ,100,101,0 ,0 ,100},
{100,0 ,0 ,0 ,100,0 ,100,0 ,0 ,0 ,0 ,0 ,0 ,100,0 ,0 ,100},
{100,101,100,0 ,0 ,0 ,100,0 ,100,100,0 ,100,0 ,0 ,100,0 ,100},
{100,100,100,0 ,100,100,100,0 ,202,0 ,0 ,100,100,0 ,100,0 ,100},
{100,0 ,100,0 ,0 ,0 ,100,0 ,0 ,100,0 ,0 ,100,0 ,0 ,100,100},
{100,0 ,0 ,0 ,100,0 ,0 ,0 ,100,0 ,100,100,100,0 ,0 ,100,100},
{100,0 ,100,0 ,100,100,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,100,100,100},
{100,100,100,0 ,100,101,100,100,101,101,100,100,100,0 ,0 ,100,100},
{100,100,101,0 ,0 ,0 ,0 ,0 ,100,100,0 ,0 ,0 ,0 ,100,100,100},
{100,0 ,0 ,0 ,100,0 ,100,101,0 ,200,0 ,100,0 ,0 ,0 ,100,100},
{100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100},
};
int gameBoardInfo2[GBOARD_HEIGHT + 2][GBOARD_WIDTH + 2] = {
{100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100},
{100,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,100},
{100,0 ,100,0 ,0 ,100,100,100,0 ,100,100,100,0 ,0 ,100,0 ,100},
{100,0 ,0 ,0 ,101,0 ,0 ,0 ,0 ,0 ,0 ,0 ,101,0 ,0 ,0 ,100},
{100,0 ,0 ,100,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,100,0 ,0 ,100},
{100,0 ,0 ,100,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,100,0 ,0 ,100},
{100,0 ,0 ,100,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,100,0 ,0 ,100},
{100,0 ,0 ,100,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,100,0 ,0 ,100},
{100,0 ,0 ,100,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,100,0 ,0 ,100},
{100,0 ,0 ,0 ,100,0 ,0 ,0 ,0 ,0 ,0 ,0 ,100,0 ,0 ,0 ,100},
{100,101,0 ,0 ,0 ,100,0 ,0 ,0 ,0 ,0 ,100,0 ,0 ,0 ,0 ,100},
{100,101,0 ,0 ,0 ,0 ,100,0 ,0 ,0 ,100,0 ,0 ,0 ,0 ,101,100},
{100,101,101,0 ,0 ,0 ,0 ,100,100,100,0 ,0 ,0 ,0 ,101,101,100},
{100,101,101,0 ,101,0 ,0 ,0 ,0 ,0 ,0 ,0 ,101,0 ,101,101,100},
{100,101,100,0 ,101,101,0 ,0 ,0 ,0 ,0 ,101,101,0 ,100,101,100},
{100,101,0 ,0 ,101,101,101,0 ,0 ,0 ,101,101,101,0 ,0 ,101,100},
{100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100},
};
int gameBoardInfo3[GBOARD_HEIGHT + 2][GBOARD_WIDTH + 2] = {
{100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100},
{100,0 ,0 ,100,101,100,101,100,101,100,101,100,101,100,0 ,0 ,100},
{100,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,100},
{100,100,0 ,100,101,100,101,100,0 ,100,101,100,101,100,0 ,100,100},
{100,101,0 ,101,100,100,100,101,0 ,101,100,100,100,101,0 ,101,100},
{100,100,0 ,100,100,100,100,100,0 ,100,100,100,100,100,0 ,100,100},
{100,101,0 ,101,100,100,100,101,0 ,101,100,100,100,101,0 ,101,100},
{100,100,0 ,100,101,100,101,100,0 ,100,101,100,101,100,0 ,100,100},
{100,101,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,101,100},
{100,100,0 ,100,101,100,101,100,0 ,100,101,100,101,100,0 ,100,100},
{100,101,0 ,101,100,100,100,101,0 ,101,100,100,100,101,0 ,101,100},
{100,100,0 ,100,100,100,100,100,0 ,100,100,100,100,100,0 ,100,100},
{100,101,0 ,101,100,100,100,101,0 ,101,100,100,100,101,0 ,101,100},
{100,101,0 ,100,101,100,101,100,0 ,100,101,100,101,100,0 ,100,100},
{100,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,100},
{100,0 ,0 ,100,101,100,101,100,101,100,101,100,101,100,0 ,0 ,100},
{100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100},
};
int gameBoardInfo4[GBOARD_HEIGHT + 2][GBOARD_WIDTH + 2] = {
{100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100},
{100,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,100},
{100,101,101,100,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,100,101,101,100},
{100,101,100,100,100,0 ,0 ,0 ,0 ,0 ,0 ,0 ,100,100,100,101,100},
{100,101,101,100,100,0 ,0 ,0 ,0 ,0 ,0 ,0 ,100,100,101,101,100},
{100,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,100},
{100,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,100},
{100,0 ,0 ,100,0 ,100,100,100,100,100,100,100,0 ,100,0 ,0 ,100},
{100,0 ,0 ,100,0 ,100,100,100,100,100,100,100,0 ,100,0 ,0 ,100},
{100,0 ,0 ,100,0 ,100,100,100,100,100,100,100,0 ,100,0 ,0 ,100},
{100,0 ,0 ,100,0 ,100,100,100,100,100,100,100,0 ,100,0 ,0 ,100},
{100,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,100},
{100,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,100},
{100,101,100,100,100,0 ,0 ,0 ,0 ,0 ,0 ,0 ,100,100,100,101,100},
{100,101,100,100,100,0 ,0 ,0 ,0 ,0 ,0 ,0 ,100,100,100,101,100},
{100,101,0 ,100,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,100,0 ,101,100},
{100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100},
};
int gameBoardInfotutorial[GBOARD_HEIGHT + 2][GBOARD_WIDTH + 2] = {
   {100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100},
   {100,0 ,0 ,0 ,0 ,0 ,0 ,0 ,101 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,100},
   {100,0 ,0 ,0 ,0 ,0 ,0 ,0 ,101 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,100},
   {100,0 ,0 ,0 ,0 ,0 ,0 ,0 ,101 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,100},
   {100,0 ,0 ,0 ,0 ,0 ,0 ,0 ,101 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,100},
   {100,0 ,0 ,0 ,0 ,0 ,0 ,0 ,101 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,100},
   {100,0 ,0 ,0 ,200 ,200 ,200 ,0 ,101 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,100},
   {100,0 ,0 ,0 ,0 ,0 ,0 ,0 ,101 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,100},
   {100,0 ,0 ,0 ,201 ,201 ,201 ,0 ,100,100,100,100,100,100,100,100,100},
   {100,0 ,0 ,0 ,0 ,0 ,0 ,0 ,101 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,100},
   {100,0 ,0 ,0 ,202 ,202 ,202 ,0 ,101 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,100},
   {100,0 ,0 ,0 ,0 ,0 ,0 ,0 ,101 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,100},
   {100,0 ,0 ,0 ,0 ,0 ,0 ,0 ,101 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,100},
   {100,0 ,0 ,0 ,0 ,0 ,0 ,0 ,101 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,100},
   {100,0 ,0 ,0 ,0 ,0 ,0 ,0 ,101 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,100},
   {100,0 ,0 ,0 ,0 ,0 ,0 ,0 ,101 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,100},
   {100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100},
};

int gameBoardInfoTest[GBOARD_HEIGHT + 2][GBOARD_WIDTH + 2] = {
{100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100},
{100,0 ,0 ,100,101,100,101,100,101,100,101,100,101,100,0 ,0 ,100},
{100,0 ,0 ,0 ,101,101,101,101,101,101,101,101,101,0 ,0 ,0 ,100},
{100,100,0 ,100,101,100,101,100,101,100,101,100,101,100,0 ,100,100},
{100,101,0,0,0,0,0,0,0,0,0,0,0,0,0,0,100},
{100,100,0,100,101,100,101,100,101,100,101,100,101,100,0,100,100},
{100,101,0,0,0,0,0,0,0,0,0,0,0,0,0,0,100},
{100,100,0,100,101,100,101,100,101,100,101,100,101,100,0,100,100},
{100,101,0,0,0,0,0,0,0,0,0,0,0,0,0,0,100},
{100,100,0,100,101,100,101,100,101,100,101,100,101,100,0,100,100},
{100,101,0,101,101,101,101,101,101,101,101,101,101,101,0,101,100},
{100,100,0,100,101,100,101,100,101,100,101,100,101,100,0,100,100},
{100,101,0,101,101,101,101,101,101,101,101,101,101,101,0,101,100},
{100,101,0 ,100,101,100,101,100,101,100,101,100,101,100,0 ,100,100},
{100,0 ,0 ,0 ,101,101,101,101,101,101,101,101,101,0 ,0 ,0 ,100},
{100,0 ,0 ,0,0,0,0,0,0,0,0,0,0,0,0 ,0 ,100},
{100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100},
};

// 보드 판에 시간을 기록하는 함수 (시간이 지나면 폭탄이 터지게 하기 위함)
void firstTimeBoardInfo(double current_time, int x, int y)
{
    TimeBoardInfo[y][x] = current_time;


}
void putBomb(int x, int y)          // 폭탄을 놓는 함수
{
    if (isBombNum(x, y) == 1)
    {

        gameBoardInfo[y][(x)] = 300;
        clock_t start = clock();
        double current_time = (double)start / CLOCKS_PER_SEC;
        firstTimeBoardInfo(current_time, x, y);
        MainCharacter.bombNum++;
    }
    else return;
}
int isBombNum(int x, int y)         // 폭탄의 갯수를 세는 함수
{

    if (gameBoardInfo[y][x] != 0)
    {

        return 0;
    }
    if (MainCharacter.bombNum >= MainCharacter.plusBombNumItem)

    {

        return 0;
    }

    return 1;
}
int isMiddleBomb(int i, int j)  // 폭탄이 아직 터지지 않는 종류인지 확인

{
    if (gameBoardInfo[i][j] >= BombFour && gameBoardInfo[i][j] <= BombOne2)
    {
        return 1;
    }
    else return 0;

}
int isFinalBomb(int i, int j)       // 터지기 직전의 폭탄인지 확인
{
    if (gameBoardInfo[i][j] == BombOne2)
    {
        return 1;
    }
    else return 0;
}
int isWaterLine(int i, int j)               // 물줄기인지 확인한다.
{
    if (gameBoardInfo[i][j] == BombZero)
        return 1;
    else return 0;

}
int detectCharacter(int i, int j)   // 탐지하고 물줄기를 맞게한다.
{
    if (gameBoardInfo[i][j] == 400 || gameBoardInfo[i][j] == 401)
    {
        MainCharacter.hp--;
        gameBoardInfo[i][j] = 401;
        return 1;
    }
    if (gameBoardInfo[i][j] == 500 || gameBoardInfo[i][j] == 501)
    {
        if (gameBoardInfo[i][j] == 500)
        {
            for (int k = 0; k < cnt_npc_pattern; k++) {
                if (npc_pattern[k].pos.X == j && npc_pattern[k].pos.Y == i && npc_pattern[k].live) {
                    npc_pattern[k].live = FALSE;//npc 죽임
                }
            }
            gameBoardInfo[i][j] = 0; //npc지움
        }
        if (gameBoardInfo[i][j] == 501)
        {
            for (int k = 0; k < cnt_npc_nopattern; k++) {
                if (npc_nopattern[k].pos.X == j && npc_nopattern[k].pos.Y == i && npc_nopattern[k].live) {
                    npc_nopattern[k].live = FALSE;//npc 죽임
                }
            }
            gameBoardInfo[i][j] = 0; //npc지움
        }
    }
    return 0;
}
int detectBlock(int i, int j)       // 블록인지 확인
{
    if (gameBoardInfo[i][j] == BlockWeak) {


        return 0;
    }
    if (gameBoardInfo[i][j] == BlockSolid)return 1;



    return 0;
}
int detectBossWeakBlock(int i, int j)       // 약한 블록인지 확인
{
    if (gameBoardInfo[i][j] == BossWeakBlock)
    {
        return 1;
    }
    return 0;
}
int makeRandomItem()                    // 아이템이 나오게하는 함수 
{
    int ran = rand() % 3;
    int itemN = 200 + ran;
    return itemN;
}
void explosion()                // 물줄기 폭발하게 하는 함수
{
    for (int i = 0; i < 17; i++)
    {
        for (int j = 0; j < 17; j++)
        {
            if (isWaterLine(i, j) == 1)
            {
                gameBoardInfo[i][j] = 0;
            }
        }
    }

    for (int i = 0; i < 17; i++)
    {
        for (int j = 0; j < 17; j++)
        {


            if (isFinalBomb(i, j) == 1)
            {
                MainCharacter.bombNum--;
                if (detectCharacter(i, j) == 0)
                    gameBoardInfo[i][j] = BombZero;
                for (int x = 1; x < MainCharacter.plusBombPowerItem; x++)
                {
                    if (gameBoardInfo[i][j + x] >= ItemHeart && gameBoardInfo[i][j + x] <= ItemPower)
                        gameBoardInfo[i][j + x] = BombZero;
                    else
                    {
                        if (detectBossWeakBlock(i, j + x) == 1)
                        {
                            bossCharacter.boss_hp--;
                            bossCharacter.phase = 2;
                        }
                        else
                        {
                            if ((detectCharacter(i, j + x) == 0 && (detectBlock(i, j + x) == 0) && gameBoardInfo[i][j + x] == 0) || gameBoardInfo[i][j + x] == BlockWeak)
                            {
                                if (gameBoardInfo[i][j + x] == BlockWeak)
                                {
                                    int ran = rand() % 3;
                                    if (ran == 0)
                                    {
                                        gameBoardInfo[i][j + x] = makeRandomItem();

                                    }
                                    else
                                    {
                                        gameBoardInfo[i][j + x] = BombZero;

                                    }
                                    break;

                                }
                                if (gameBoardInfo[i][j + x]<ItemHeart || gameBoardInfo[i][j + x]>ItemPower)

                                    gameBoardInfo[i][j + x] = BombZero;




                            }
                            else
                                break;
                        }
                    }
                }
                for (int x = 1; x < MainCharacter.plusBombPowerItem; x++)
                {
                    if (gameBoardInfo[i][j - x] >= ItemHeart && gameBoardInfo[i][j - x] <= ItemPower)
                        gameBoardInfo[i][j - x] = BombZero;
                    else
                    {
                        if (detectBossWeakBlock(i, j - x) == 1)
                        {
                            bossCharacter.boss_hp--;
                            bossCharacter.phase = 2;
                        }
                        else
                        {


                            if ((detectCharacter(i, j - x) == 0 && (detectBlock(i, j - x) == 0) && gameBoardInfo[i][j - x] == 0) || gameBoardInfo[i][j - x] == BlockWeak)
                            {
                                if (gameBoardInfo[i][j - x] == BlockWeak)
                                {
                                    int ran = rand() % 3;
                                    if (ran == 0)
                                    {
                                        gameBoardInfo[i][j - x] = makeRandomItem();

                                    }
                                    else
                                    {
                                        gameBoardInfo[i][j - x] = BombZero;

                                    }
                                    break;
                                }
                                if (gameBoardInfo[i][j - x]<ItemHeart || gameBoardInfo[i][j - x]>ItemPower)
                                    gameBoardInfo[i][j - x] = BombZero;

                            }
                            else
                                break;
                        }
                    }
                }
                for (int x = 1; x < MainCharacter.plusBombPowerItem; x++)
                {
                    if (gameBoardInfo[i + x][j] >= ItemHeart && gameBoardInfo[i + x][j] <= ItemPower)
                        gameBoardInfo[i + x][j] = BombZero;
                    else
                    {

                        if (detectBossWeakBlock(i + x, j) == 1)
                        {

                            bossCharacter.boss_hp--;
                            bossCharacter.phase = 2;
                        }
                        else
                        {
                            if ((detectCharacter(i + x, j) == 0 && (detectBlock(i + x, j) == 0) && gameBoardInfo[i + x][j] == 0) || gameBoardInfo[i + x][j] == BlockWeak)
                            {
                                if (gameBoardInfo[i + x][j] == BlockWeak)
                                {
                                    int ran = rand() % 3;
                                    if (ran == 0)
                                    {
                                        gameBoardInfo[i + x][j] = makeRandomItem();

                                    }
                                    else
                                    {
                                        gameBoardInfo[i + x][j] = BombZero;

                                    }
                                    break;
                                }
                                if (gameBoardInfo[i + x][j]<ItemHeart || gameBoardInfo[i + x][j]>ItemPower)
                                    gameBoardInfo[i + x][j] = BombZero;

                            }
                            else
                                break;
                        }
                    }
                }
                for (int x = 1; x < MainCharacter.plusBombPowerItem; x++)
                {
                    if (gameBoardInfo[i - x][j] >= ItemHeart && gameBoardInfo[i - x][j] <= ItemPower)
                        gameBoardInfo[i - x][j] = BombZero;
                    else
                    {

                        if (detectBossWeakBlock(i - x, j) == 1)
                        {
                            bossCharacter.boss_hp--;
                            bossCharacter.phase = 2;
                        }
                        else
                        {
                            if ((detectCharacter(i - x, j) == 0 && (detectBlock(i - x, j) == 0) && gameBoardInfo[i - x][j] == 0) || gameBoardInfo[i - x][j] == BlockWeak)
                            {
                                if (gameBoardInfo[i - x][j] == BlockWeak)
                                {
                                    int ran = rand() % 3;
                                    if (ran == 0)
                                    {
                                        gameBoardInfo[i - x][j] = makeRandomItem();

                                    }
                                    else
                                    {
                                        gameBoardInfo[i - x][j] = BombZero;
                                    }
                                    break;
                                }
                                if (gameBoardInfo[i - x][j]<ItemHeart || gameBoardInfo[i - x][j]>ItemPower)
                                    gameBoardInfo[i - x][j] = BombZero;

                            }
                            else
                                break;
                        }
                    }
                }
                drawingTotalMap();







            }
        }
    }
}


void findChangingBomb(double current_time)      // 바뀌는 봄브 즉 터지지 않은 봄브를 찾아
{                                               // 다음 봄브로 바뀌게 하는 함수
    for (int i = 0; i < 17; i++)
    {
        for (int j = 0; j < 17; j++)
        {
            if (isMiddleBomb(i, j) == 1)

            {




                if (current_time - TimeBoardInfo[i][j] >= 5.6)
                {
                    gameBoardInfo[i][j] = BombOne2;
                }

                if (current_time - TimeBoardInfo[i][j] >= 4.2 && current_time - TimeBoardInfo[i][j] < 4.9)
                {

                    gameBoardInfo[i][j] = BombOne;



                }
                if (current_time - TimeBoardInfo[i][j] >= 3.5 && current_time - TimeBoardInfo[i][j] < 4.2)
                {

                    gameBoardInfo[i][j] = BombTwo2;
                }
                if (current_time - TimeBoardInfo[i][j] >= 2.8 && current_time - TimeBoardInfo[i][j] < 3.5)
                {

                    gameBoardInfo[i][j] = BombTwo;
                }
                if (current_time - TimeBoardInfo[i][j] >= 2.1 && current_time - TimeBoardInfo[i][j] < 2.8)
                {

                    gameBoardInfo[i][j] = BombThree2;
                }
                if (current_time - TimeBoardInfo[i][j] >= 1.4 && current_time - TimeBoardInfo[i][j] < 2.1)
                {

                    gameBoardInfo[i][j] = BombThree;
                }
                if (current_time - TimeBoardInfo[i][j] >= 0.7 && current_time - TimeBoardInfo[i][j] < 1.4)
                {

                    gameBoardInfo[i][j] = BombFour2;
                }


                drawingTotalMap();

            }
        }
    }
}

void SetCurrentCursorPos(int x, int y)      // 커서 위치를 세팅하는 함수
{
    COORD position = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
}
COORD GetCurrentCursorPos(void)             // 커서 위치를 얻어내는 함수
{
    COORD curPoint;
    CONSOLE_SCREEN_BUFFER_INFO curInfo;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
    curPoint.X = curInfo.dwCursorPosition.X;
    curPoint.Y = curInfo.dwCursorPosition.Y;
    return curPoint;
}
void RemoveCursor(void)             // 커서를 안보이게 하는 함수

{

    CONSOLE_CURSOR_INFO curInfo;

    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);

    curInfo.bVisible = 0;

    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);

}

//한강섭
int abs(int n) {                    // 절댓값 얻는 함수
    if (n < 0) return n * -1;
    else return n;
}

void setpc(int y, int x) {              // pc 위치를 설정하는 함수
    pc->pos.Y = y;
    pc->pos.X = x;
    gameBoardInfo[y][x] = 400;
    return;
}
void setnpc_pattern(int i, int y, int x) {      // npc1 위치를 설정하는 함수
    npc_pattern[i].live = TRUE;
    npc_pattern[i].pos.Y = y;
    npc_pattern[i].pos.X = x;
    gameBoardInfo[y][x] = 500;
    return;
}
void setnpc_nopattern(int i, int y, int x) {        // npc2 위치를 설정하는 함수
    npc_pattern[i].live = TRUE; 
    npc_nopattern[i].pos.Y = y;
    npc_nopattern[i].pos.X = x;
    gameBoardInfo[y][x] = 501;
    return;
}

int DetectpcCollision(int y, int x) {           // 물줄기의 부딪힘을 탐지하는 함수
    if (gameBoardInfo[y][x] == 0) return 0;//아무것도 없는 곳이니깐 움직인다
    else if (gameBoardInfo[y][x] == 500 || gameBoardInfo[y][x] == 501) { //다음 위치가 npc랑 만나는 위치일때
        MainCharacter.hp--;
        if (gameBoardInfo[y][x] == 500)
        {
            for (int i = 0; i < cnt_npc_pattern; i++) {
                if (npc_pattern[i].pos.X == x && npc_pattern[i].pos.Y == y && npc_pattern[i].live) {
                    npc_pattern[i].live = FALSE;//npc 죽임
                }
            }
            gameBoardInfo[y][x] = 0; //npc지움
        }
        if (gameBoardInfo[y][x] == 501)
        {
            for (int i = 0; i < cnt_npc_nopattern; i++) {
                if (npc_nopattern[i].pos.X == x && npc_nopattern[i].pos.Y == y && npc_nopattern[i].live) {
                    npc_nopattern[i].live = FALSE; //npc죽임
                }
            }
            gameBoardInfo[y][x] = 0; //지운다.
        }
        return 2; //pc를 다음위치로 움직인다.
    }
    else if (gameBoardInfo[y][x] == 200) {
        if (MainCharacter.hp < 6) {
            MainCharacter.hp += 1;
        }
        gameBoardInfo[y][x] = 0;
        return 0;
    }
    else if (gameBoardInfo[y][x] == 201) {
        if (MainCharacter.plusBombNumItem < 10) {
            MainCharacter.plusBombNumItem += 1;
        }
        gameBoardInfo[y][x] = 0;
        return 0;
    }
    else if (gameBoardInfo[y][x] == 202) {
        if (MainCharacter.plusBombPowerItem < 11) {
            MainCharacter.plusBombPowerItem += 1;
        }
        gameBoardInfo[y][x] = 0;
        return 0;
    }
    else return 1; //그 외에는 움직이지 않는다.
}
int DetectnpcCollision(int y, int x) {
    if (gameBoardInfo[y][x] == 0) return 0;//아무것도 없는 곳이니깐 움직인다
    else if (gameBoardInfo[y][x] == 400) return 2; //다음 위치가 pc랑 만나는 위치일때
    else return 1; //막혀있을 때
}

void move_pc(int y, int x) {        // pc를 움직이게 하는 함수
    int isDetect = DetectpcCollision(pc->pos.Y + y, pc->pos.X + x);
    if (isDetect == 1) return;
    gameBoardInfo[pc->pos.Y][pc->pos.X] = 0;
    pc->pos.X += x;
    pc->pos.Y += y;
    if (isDetect == 2)gameBoardInfo[pc->pos.Y][pc->pos.X] = 402;
    else gameBoardInfo[pc->pos.Y][pc->pos.X] = 400;
    drawingTotalMap();
    return;
}
void move_pattern_npc() {       // npc1의 움직임을 담당하는 함수
    for (int i = 0; i < cnt_npc_pattern; i++)
    {
        if (!npc_pattern[i].live) continue;
        int random;
        int x, y;
        random = rand() % 6; //숫자를 높이면 더 사람을 따라간다.
        if (random < 4) {
            random = rand() % 4; //random 0 왼 1 오 2 위 3 아래
            if (random == 0) {
                x = -1;
                y = 0;
            }
            else if (random == 1) {
                x = 1;
                y = 0;
            }
            else if (random == 2) {
                x = 0;
                y = -1;
            }
            else {
                x = 0;
                y = 1;
            }
        }
        else {
            y = pc->pos.Y - npc_nopattern[i].pos.Y;
            x = pc->pos.X - npc_nopattern[i].pos.X;
            if (abs(y) > abs(x)) {
                if (y < 0) {
                    x = 0;
                    y = -1;
                }
                else {
                    x = 0;
                    y = 1;
                }
            }
            else {
                if (x < 0) {
                    y = 0;
                    x = -1;
                }
                else {
                    y = 0;
                    x = 1;
                }
            }
        }
        if (DetectnpcCollision(npc_pattern[i].pos.Y + y, npc_pattern[i].pos.X + x) == 1) continue; //막혔을때
        else if (DetectnpcCollision(npc_pattern[i].pos.Y + y, npc_pattern[i].pos.X + x) == 2) { //pc랑 부딪혔을때
            MainCharacter.hp--;
            gameBoardInfo[npc_pattern[i].pos.Y][npc_pattern[i].pos.X] = 0;
            npc_pattern[i].live = FALSE;
            gameBoardInfo[pc->pos.Y][pc->pos.X] = 402;
        }
        else { //움직일때
            gameBoardInfo[npc_pattern[i].pos.Y][npc_pattern[i].pos.X] = 0;
            npc_pattern[i].pos.X += x;
            npc_pattern[i].pos.Y += y;
            gameBoardInfo[npc_pattern[i].pos.Y][npc_pattern[i].pos.X] = 500;
        }
        drawingTotalMap();
    }
    return;
}
void move_nopattern_npc() {         // npc2의 움직임을 담당하는 함수
    for (int i = 0; i < cnt_npc_nopattern; i++)
    {
        if (!npc_nopattern[i].live) continue; //살아있는 것만 작동시킨다.
        int random;
        int x, y;
        random = rand() % 4; //숫자를 높이면 더 pc을 따라간다.
        if (random < 3) {
            random = rand() % 4; //random 0 왼 1 오 2 위 3 아래
            if (random == 0) {
                x = -1;
                y = 0;
            }
            else if (random == 1) {
                x = 1;
                y = 0;
            }
            else if (random == 2) {
                x = 0;
                y = -1;
            }
            else {
                x = 0;
                y = 1;
            }
        }
        else {
            y = pc->pos.Y - npc_nopattern[i].pos.Y;
            x = pc->pos.X - npc_nopattern[i].pos.X;
            if (abs(y) > abs(x)) {
                if (y < 0) {
                    x = 0;
                    y = -1;
                }
                else {
                    x = 0;
                    y = 1;
                }
            }
            else {
                if (x < 0) {
                    y = 0;
                    x = -1;
                }
                else {
                    y = 0;
                    x = 1;
                }
            }
        }
        if (DetectnpcCollision(npc_nopattern[i].pos.Y + y, npc_nopattern[i].pos.X + x) == 1) continue; //막혔을때
        else if (DetectnpcCollision(npc_nopattern[i].pos.Y + y, npc_nopattern[i].pos.X + x) == 2) { //pc랑 부딪혔을때
            MainCharacter.hp--;
            gameBoardInfo[npc_nopattern[i].pos.Y][npc_nopattern[i].pos.X] = 0;
            npc_nopattern[i].live = FALSE;
            gameBoardInfo[pc->pos.Y][pc->pos.X] = 402;

        }
        else { //움직일때
            gameBoardInfo[npc_nopattern[i].pos.Y][npc_nopattern[i].pos.X] = 0;
            npc_nopattern[i].pos.X += x;
            npc_nopattern[i].pos.Y += y;
            gameBoardInfo[npc_nopattern[i].pos.Y][npc_nopattern[i].pos.X] = 501;
        }
        drawingTotalMap();
    }
    return;
}
//npc가 모두 죽을떄
BOOLEAN npc_alldiecheck() {         // npc가 모두 죽었는지 확인하는 함수
    int flag = 0;
    for (int i = 0; i < cnt_npc_nopattern; i++) {
        if (npc_nopattern[i].live) flag = 1;
    }
    for (int i = 0; i < cnt_npc_pattern; i++) {
        if (npc_pattern[i].live) flag = 1;
    }
    if (flag == 0) return TRUE;
    else return FALSE;
}
int countnpc() {                    // npc의 수를 세는 함수
    int cnt = 0;
    for (int i = 0; i < cnt_npc_nopattern; i++) {
        if (npc_nopattern[i].live)cnt++;
    }
    for (int i = 0; i < cnt_npc_pattern; i++) {
        if (npc_pattern[i].live)cnt++;
    }
    return cnt;
}
//나중에 pc체력이 0일때도 작성
//보스 공격 할 거 만들어 논거
void spawntutorialnpc() {           // 튜토리얼때 npc가 나오게 하는 함수
    cnt_npc_nopattern = 1;
    cnt_npc_pattern = 1;
    npc_pattern = malloc(sizeof(NPC_pos_pattern) * cnt_npc_pattern);
    npc_nopattern = malloc(sizeof(NPC_pos_nopattern) * cnt_npc_nopattern);
    setnpc_nopattern(0, 2, 12);
    setnpc_pattern(0, 12, 12);
    return;
}
void spawnnpc(int n) { //n*2개의 npc 만큼 랜덤한 장소에 소환
    int y, x;
    cnt_npc_pattern = n;
    cnt_npc_nopattern = n;
    npc_pattern = malloc(sizeof(NPC_pos_pattern) * cnt_npc_pattern);
    npc_nopattern = malloc(sizeof(NPC_pos_nopattern) * cnt_npc_nopattern);
    for (int i = 0; i < n; i++) {
        while (1) {
            y = rand() % GBOARD_HEIGHT + 1;
            x = rand() % GBOARD_WIDTH + 1;
            if (gameBoardInfo[y][x] == 0) break;
        }
        setnpc_pattern(i, y, x);
    }
    for (int i = 0; i < n; i++) {
        while (1) {
            y = rand() % GBOARD_HEIGHT + 1;
            x = rand() % GBOARD_WIDTH + 1;
            if (gameBoardInfo[y][x] == 0) break;
        }
        setnpc_nopattern(i, y, x);
    }
    return;
}
void addspawnnpc(int n) {       // npc를 추가하는 함수
    int y, x;
    cnt_npc_pattern += n;
    cnt_npc_nopattern += n;
    NPC_pos_pattern* temp = malloc(sizeof(NPC_pos_pattern) * (cnt_npc_pattern));
    NPC_pos_nopattern* tempno = malloc(sizeof(NPC_pos_nopattern) * (cnt_npc_nopattern));
    for (int i = 0; i < cnt_npc_pattern - n; i++) {
        temp[i] = npc_pattern[i];
        tempno[i] = npc_nopattern[i];
    }
    free(npc_nopattern);
    free(npc_pattern);

    for (int i = cnt_npc_pattern - n; i < cnt_npc_pattern; i++) {
        while (1) {
            y = rand() % GBOARD_HEIGHT + 1;
            x = rand() % GBOARD_WIDTH + 1;
            if (gameBoardInfo[y][x] == 0) break;
        }
        temp[i].live = TRUE;
        temp[i].pos.Y = y;
        temp[i].pos.X = x;
        gameBoardInfo[y][x] = 500;
    }
    for (int i = cnt_npc_pattern - n; i < cnt_npc_pattern; i++) {
        while (1) {
            y = rand() % GBOARD_HEIGHT + 1;
            x = rand() % GBOARD_WIDTH + 1;
            if (gameBoardInfo[y][x] == 0) break;
        }
        tempno[i].live = TRUE;
        tempno[i].pos.Y = y;
        tempno[i].pos.X = x;
        gameBoardInfo[y][x] = 501;
    }
    npc_nopattern = tempno;
    npc_pattern = temp;
    drawingTotalMap();
    return;
}
void deletenpc() {          // npc를 탐지하는 함수
    int x, y;
    for (y = 0; y < GBOARD_HEIGHT + 2; y++)
    {
        for (x = 0; x < GBOARD_WIDTH + 2; x++)
        {
            if (gameBoardInfo[y][x] == 501 || gameBoardInfo[y][x] == 500) {
                gameBoardInfo[y][x] = 0;
            }
        }
    }
    for (int i = 0; i < cnt_npc_nopattern; i++) {
        npc_nopattern[i].live = FALSE;
        npc_pattern[i].live = FALSE;
    }
    return;
}
void spawnbomb(int n) {//n개의 물풍선을 랜덤한 장소에 소환
    int y, x;
    for (int i = 0; i < n; i++) {
        while (1) {
            y = rand() % GBOARD_HEIGHT + 1;
            x = rand() % GBOARD_WIDTH + 1;
            if (gameBoardInfo[y][x] == 0) break;
        }
        gameBoardInfo[y][(x)] = 300;
        clock_t start = clock();
        double current_time = (double)start / CLOCKS_PER_SEC;
        firstTimeBoardInfo(current_time, x, y);
    }
    return;
}
void spawnbossweak(int n) {     // 보스의 약점 블록을 생성하는 함수
    int y, x;
    for (int i = 0; i < n; i++) {
        while (1) {
            y = rand() % GBOARD_HEIGHT + 1;
            x = rand() % GBOARD_WIDTH + 1;
            if (gameBoardInfo[y][x] == 0) break;
        }
        gameBoardInfo[y][x] = 130;
    }
    return;
}
void deletebossweak() {     // 보스의 약점 블록을 때렸는지 확인하는 함수
    int x, y;
    for (y = 0; y < GBOARD_HEIGHT + 2; y++)
    {
        for (x = 0; x < GBOARD_WIDTH + 2; x++)
        {
            if (gameBoardInfo[y][x] == 130) {
                gameBoardInfo[y][x] = 0;
            }
        }
    }
    return;
}
void npcspeedup() {                     // npc의 속도를 높이는 함수
    if (npcspeed <= 0) return;
    npcspeed--;
    return;
}

int flag = 1; //boss 단계 구분하는 flag
int before_key;
int e = 0; //공격이랑 npc속도 관련
void ProcessKeyInput() {                // 키의 입력을 받는 함수
    int key;

    for (int i = 0; i < 20; i++) {

        if (_kbhit() != 0) {
            key = _getch();
            switch (key) {
            case 75:
                move_pc(0, -1);
                break;
            case 77:
                move_pc(0, 1);
                break;
            case 57:
                deletenpc();
                break;
            case 72:
                move_pc(-1, 0);
                break;
            case 80:
                move_pc(1, 0);
                break;
            case 32:
            {
                if (before_key == 77)
                    putBomb(pc->pos.X + 1, pc->pos.Y);
                if (before_key == 75)
                    putBomb(pc->pos.X - 1, pc->pos.Y);
                if (before_key == 72)
                    putBomb(pc->pos.X, pc->pos.Y - 1);
                if (before_key == 80)
                    putBomb(pc->pos.X, pc->pos.Y + 1);
                if (before_key == 0)
                    putBomb(pc->pos.X, pc->pos.Y + 1);
                break;
            }
            }

            before_key = key;
        }

        if (e % npcspeed == 0) //npc 의 속도 조정
        {
            move_pattern_npc();
            move_nopattern_npc();
            drawingTotalMap();
        }
        if (flag == 2) { //보스 단계일때 보스공격
            if (e % 300 == 270) {
                bossCharacter.phase = 1;
            }
            if (e % 300 == 0) {
                addspawnnpc(2);
            }
            if (e % 300 == 50) {
                bossCharacter.phase = 0;
            }
            if (e % 300 == 105) {
                bossCharacter.phase = 0;
            }
            if (e % 300 == 160) {
                bossCharacter.phase = 0;
            }
            if (e % 300 == 215) {
                bossCharacter.phase = 0;
            }
            if (e % 5000 == 0) {
                npcspeedup();
            }
            if (e % 600 == 0) {
                deletebossweak();
                spawnbossweak(4);
            }
        }

        e++;

        Sleep(20);
    }
    return;
}
int main() {
    int key, i, j;
    settingUiInit();                // UI를 세팅한다.
    srand(time(NULL));
    RemoveCursor();                 // 커서를 지운다.
    Sleep(1000);
    pc = malloc(sizeof(PC_pos));

    while (1) {
        stageNum = 0;//0은 튜토리얼 시작
        for (i = 0; i < GBOARD_HEIGHT + 2; i++) {
            for (j = 0; j < GBOARD_WIDTH + 2; j++) {
                gameBoardInfo[i][j] = gameBoardInfotutorial[i][j]; //튜토리얼로 시작함
            }
        }
        erazeWindow();
        while (1) { // 메뉴입니다.
            drawingGameMenu();
            if (_kbhit() != 0) {
                key = _getch();
                switch (key) {
                case 72:
                    menuUp();
                    break;
                case 80:
                    menuDown();
                    break;
                case 32:
                    if (curMenu == 0) {
                        whatClick = 1;
                    }
                    else {
                        whatClick = 2;
                    }
                    break;
                }
            }
            if (whatClick == 1) {
                whatClick = 0;
                break;
            }
            else if (whatClick == 2) {
                whatClick = 0;
                wantExit = 1;
                break;
            }
        }

        if (wantExit == 1) {
            break;
        }

        if (stageNum == 0) spawntutorialnpc();
        else spawnnpc(3);
        setpc(1, 1); // pc 위치 초기화
        //각각 3개의 npc를 소환하는 함수 만듬
        MainCharacter.bombNum = 0;
        MainCharacter.plusBombNumItem = 3; // 물폭탄 개수 초기화
        MainCharacter.plusBombPowerItem = 2; // 화력 초기화
        MainCharacter.hp = 3;
        bossCharacter.boss_hp = 10;
        // pc hp 초기화
        erazeWindow();
        drawingTotalMap();
        while (1) {
            //printf("%lf", time);
            clock_t current_time = clock();
            double time = (double)current_time / CLOCKS_PER_SEC;
            findChangingBomb(time);
            explosion();
            ProcessKeyInput();
            /* 스테이지 ㄱㄱ
            */
            if (MainCharacter.hp < 1)
            {
                flag = 1;
                break; //npc가 모두 죽으면 끝내준다. or pc 죽으면 끝내준다.
            }
            if (flag == 1) { //보스 맵 아닐때
                if (npc_alldiecheck()) {
                    free(npc_pattern);
                    free(npc_nopattern);
                    flag = nextStage();
                    if (flag == 0) {
                        break; //다음스테이지가 없다면 break
                    }
                }
            }
            else { //보스 맵일때

                if (bossCharacter.boss_hp == 0) {
                    flag = nextStage();
                    if (flag == 0) {
                        break; //다음스테이지가 없다면 break
                    }
                }
            }

        }
    }

    return 0;
}

void settingUiInit() {          // UI를 세팅하는 함수 (아직 필요없지만 후의 버전 증가때를 위해)
}

void drawingTotalMap() {        // MAP을 모두 그려준다.
    printGameBoard();
    printHeroHp();
    drawBombNumUI();
    drawPowerUI();
    drawNpcHP();
}

void printGameBoard() {     // 게임 보드를 그린다.
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
                    if (stageNum == 4 && x >= BOSS_START_X && y >= BOSS_START_Y && x <= 11 && y <= 10) {
                        if (bossCharacter.boss_hp > 8) {
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GREEN);
                        }
                        else if (bossCharacter.boss_hp > 6) {
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), YELLOW);
                        }
                        else if (bossCharacter.boss_hp > 4) {
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DarkYellow);
                        }
                        else if (bossCharacter.boss_hp > 2) {
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), RED);
                        }
                        else {
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DarkRed);
                        }
                        if (bossCharacter.phase == 0) {
                            if (x == BOSS_START_X && y == BOSS_START_Y) {
                                printf("┌ ");
                            }
                            else if (x == BOSS_START_X + 1 && y == BOSS_START_Y) {
                                printf("──");
                            }
                            else if (x == BOSS_START_X + 2 && y == BOSS_START_Y) {
                                printf("──");
                            }
                            else if (x == BOSS_START_X + 3 && y == BOSS_START_Y) {
                                printf("──");
                            }
                            else if (x == BOSS_START_X + 4 && y == BOSS_START_Y) {
                                printf("──");
                            }
                            else if (x == BOSS_START_X + 5 && y == BOSS_START_Y) {
                                printf(" ┐");
                            }
                            else if (x == BOSS_START_X + 6 && y == BOSS_START_Y) {
                                printf("  ");
                            }
                            else if (x == BOSS_START_X && y == BOSS_START_Y + 1) {
                                printf("O ");
                            }
                            else if (x == BOSS_START_X + 1 && y == BOSS_START_Y + 1) {
                                printf("| ");
                            }
                            else if (x == BOSS_START_X + 2 && y == BOSS_START_Y + 1) {
                                printf("o ");
                            }
                            else if (x == BOSS_START_X + 3 && y == BOSS_START_Y + 1) {
                                printf("@ ");
                            }
                            else if (x == BOSS_START_X + 4 && y == BOSS_START_Y + 1) {
                                printf("o ");
                            }
                            else if (x == BOSS_START_X + 5 && y == BOSS_START_Y + 1) {
                                printf("| ");
                            }
                            else if (x == BOSS_START_X + 6 && y == BOSS_START_Y + 1) {
                                printf("O ");
                            }
                            else if (x == BOSS_START_X && y == BOSS_START_Y + 2) {
                                printf("└ ");
                            }
                            else if (x == BOSS_START_X + 1 && y == BOSS_START_Y + 2) {
                                printf("──");
                            }
                            else if (x == BOSS_START_X + 2 && y == BOSS_START_Y + 2) {
                                printf("──");
                            }
                            else if (x == BOSS_START_X + 3 && y == BOSS_START_Y + 2) {
                                printf("──");
                            }
                            else if (x == BOSS_START_X + 4 && y == BOSS_START_Y + 2) {
                                printf("──");
                            }
                            else if (x == BOSS_START_X + 5 && y == BOSS_START_Y + 2) {
                                printf("─┘");
                            }
                            else if (x == BOSS_START_X + 6 && y == BOSS_START_Y + 2) {
                                printf("  ");
                            }
                            else if (x == BOSS_START_X && y == BOSS_START_Y + 3) {
                                printf("∮");
                            }
                            else if (x == BOSS_START_X + 1 && y == BOSS_START_Y + 3) {
                                printf("∮");
                            }
                            else if (x == BOSS_START_X + 2 && y == BOSS_START_Y + 3) {
                                printf("∮");
                            }
                            else if (x == BOSS_START_X + 3 && y == BOSS_START_Y + 3) {
                                printf("∮");
                            }
                            else if (x == BOSS_START_X + 4 && y == BOSS_START_Y + 3) {
                                printf("∮");
                            }
                            else if (x == BOSS_START_X + 5 && y == BOSS_START_Y + 3) {
                                printf("∮");
                            }
                            else if (x == BOSS_START_X + 6 && y == BOSS_START_Y + 3) {
                                printf("∮");
                            }
                        }
                        else if (bossCharacter.phase == 1) {
                            if (x == BOSS_START_X && y == BOSS_START_Y) {
                                printf("┌ ");
                            }
                            else if (x == BOSS_START_X + 1 && y == BOSS_START_Y) {
                                printf("──");
                            }
                            else if (x == BOSS_START_X + 2 && y == BOSS_START_Y) {
                                printf("──");
                            }
                            else if (x == BOSS_START_X + 3 && y == BOSS_START_Y) {
                                printf("──");
                            }
                            else if (x == BOSS_START_X + 4 && y == BOSS_START_Y) {
                                printf("──");
                            }
                            else if (x == BOSS_START_X + 5 && y == BOSS_START_Y) {
                                printf(" ┐");
                            }
                            else if (x == BOSS_START_X + 6 && y == BOSS_START_Y) {
                                printf("  ");
                            }
                            else if (x == BOSS_START_X && y == BOSS_START_Y + 1) {
                                printf(" ┌");
                            }
                            else if (x == BOSS_START_X + 1 && y == BOSS_START_Y + 1) {
                                printf(" |");
                            }
                            else if (x == BOSS_START_X + 2 && y == BOSS_START_Y + 1) {
                                printf("Φ");
                            }
                            else if (x == BOSS_START_X + 3 && y == BOSS_START_Y + 1) {
                                printf("言");
                            }
                            else if (x == BOSS_START_X + 4 && y == BOSS_START_Y + 1) {
                                printf("Φ");
                            }
                            else if (x == BOSS_START_X + 5 && y == BOSS_START_Y + 1) {
                                printf("|");
                            }
                            else if (x == BOSS_START_X + 6 && y == BOSS_START_Y + 1) {
                                printf("ノ");
                            }
                            else if (x == BOSS_START_X && y == BOSS_START_Y + 2) {
                                printf("└ ");
                            }
                            else if (x == BOSS_START_X + 1 && y == BOSS_START_Y + 2) {
                                printf("──");
                            }
                            else if (x == BOSS_START_X + 2 && y == BOSS_START_Y + 2) {
                                printf("──");
                            }
                            else if (x == BOSS_START_X + 3 && y == BOSS_START_Y + 2) {
                                printf("──");
                            }
                            else if (x == BOSS_START_X + 4 && y == BOSS_START_Y + 2) {
                                printf("──");
                            }
                            else if (x == BOSS_START_X + 5 && y == BOSS_START_Y + 2) {
                                printf("─┘");
                            }
                            else if (x == BOSS_START_X + 6 && y == BOSS_START_Y + 2) {
                                printf("  ");
                            }
                            else if (x == BOSS_START_X && y == BOSS_START_Y + 3) {
                                printf("∮");
                            }
                            else if (x == BOSS_START_X + 1 && y == BOSS_START_Y + 3) {
                                printf("∮");
                            }
                            else if (x == BOSS_START_X + 2 && y == BOSS_START_Y + 3) {
                                printf("∮");
                            }
                            else if (x == BOSS_START_X + 3 && y == BOSS_START_Y + 3) {
                                printf("∮");
                            }
                            else if (x == BOSS_START_X + 4 && y == BOSS_START_Y + 3) {
                                printf("∮");
                            }
                            else if (x == BOSS_START_X + 5 && y == BOSS_START_Y + 3) {
                                printf("∮");
                            }
                            else if (x == BOSS_START_X + 6 && y == BOSS_START_Y + 3) {
                                printf("∮");
                            }
                        }
                        else if (bossCharacter.phase == 2) {
                            if (x == BOSS_START_X && y == BOSS_START_Y) {
                                printf("┌ ");
                            }
                            else if (x == BOSS_START_X + 1 && y == BOSS_START_Y) {
                                printf("──");
                            }
                            else if (x == BOSS_START_X + 2 && y == BOSS_START_Y) {
                                printf("──");
                            }
                            else if (x == BOSS_START_X + 3 && y == BOSS_START_Y) {
                                printf("──");
                            }
                            else if (x == BOSS_START_X + 4 && y == BOSS_START_Y) {
                                printf("──");
                            }
                            else if (x == BOSS_START_X + 5 && y == BOSS_START_Y) {
                                printf(" ┐");
                            }
                            else if (x == BOSS_START_X + 6 && y == BOSS_START_Y) {
                                printf("  ");
                            }
                            else if (x == BOSS_START_X && y == BOSS_START_Y + 1) {
                                printf("o");
                            }
                            else if (x == BOSS_START_X + 1 && y == BOSS_START_Y + 1) {
                                printf("Σ|");
                            }
                            else if (x == BOSS_START_X + 2 && y == BOSS_START_Y + 1) {
                                printf("T");
                            }
                            else if (x == BOSS_START_X + 3 && y == BOSS_START_Y + 1) {
                                printf("ω");
                            }
                            else if (x == BOSS_START_X + 4 && y == BOSS_START_Y + 1) {
                                printf("T");
                            }
                            else if (x == BOSS_START_X + 5 && y == BOSS_START_Y + 1) {
                                printf("|");
                            }
                            else if (x == BOSS_START_X + 6 && y == BOSS_START_Y + 1) {
                                printf("o");
                            }
                            else if (x == BOSS_START_X && y == BOSS_START_Y + 2) {
                                printf("└ ");
                            }
                            else if (x == BOSS_START_X + 1 && y == BOSS_START_Y + 2) {
                                printf("──");
                            }
                            else if (x == BOSS_START_X + 2 && y == BOSS_START_Y + 2) {
                                printf("──");
                            }
                            else if (x == BOSS_START_X + 3 && y == BOSS_START_Y + 2) {
                                printf("──");
                            }
                            else if (x == BOSS_START_X + 4 && y == BOSS_START_Y + 2) {
                                printf("──");
                            }
                            else if (x == BOSS_START_X + 5 && y == BOSS_START_Y + 2) {
                                printf("─┘");
                            }
                            else if (x == BOSS_START_X + 6 && y == BOSS_START_Y + 2) {
                                printf("  ");
                            }
                            else if (x == BOSS_START_X && y == BOSS_START_Y + 3) {
                                printf("∮");
                            }
                            else if (x == BOSS_START_X + 1 && y == BOSS_START_Y + 3) {
                                printf("∮");
                            }
                            else if (x == BOSS_START_X + 2 && y == BOSS_START_Y + 3) {
                                printf("∮");
                            }
                            else if (x == BOSS_START_X + 3 && y == BOSS_START_Y + 3) {
                                printf("∮");
                            }
                            else if (x == BOSS_START_X + 4 && y == BOSS_START_Y + 3) {
                                printf("∮");
                            }
                            else if (x == BOSS_START_X + 5 && y == BOSS_START_Y + 3) {
                                printf("∮");
                            }
                            else if (x == BOSS_START_X + 6 && y == BOSS_START_Y + 3) {
                                printf("∮");
                            }
                        }
                    }
                    else {
                        printf("■");
                    }
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
                    break;
                case BlockWeak:
                    printf("▦");
                    break;
                case BossWeakBlock:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), PURPLE);
                    printf("▣");
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
                    break;
                case BossWeakBlockAttacked:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
                    printf("▣");
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
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
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), SkyBlue);
                    printf("Θ");
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
                    break;
                case BombFour2:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), SkyBlue);
                    printf("ㅇ");
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
                    break;
                case BombThree:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), darkSkyBlue);
                    printf("Θ");
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
                    break;
                case BombThree2:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), darkSkyBlue);
                    printf("ㅇ");
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
                    break;
                case BombTwo:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BLUE);
                    printf("Θ");
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
                    break;
                case BombTwo2:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BLUE);
                    printf("ㅇ");
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
                    break;
                case BombOne:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), darkBLUE);
                    printf("Θ");
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
                    break;
                case BombOne2:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), darkBLUE);
                    printf("ㅇ");
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
                    break;
                case BombZero:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BLUE);
                    printf("※");
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
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
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BLUE);
                    printf("♀");
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
                    break;
                case PcOnHit:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), RED);
                    printf("♀");
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
                    break;
                default:
                    break;
                }
            }
            else if (500 <= gameBoardInfo[y][x] && gameBoardInfo[y][x] < 600) {
                switch (gameBoardInfo[y][x]) {
                case NpcNOPattern:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), YELLOW);
                    printf("§");
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
                    break;
                case NpcPattern:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), RED);
                    printf("※");
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
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

void printHeroHp() {        // PC의 HP를 그린다.
    SetCurrentCursorPos(STATUS_MENU_WINDOW_X, STATUS_MENU_WINDOW_Y);
    if (MainCharacter.hp == 6) {
        printf("플레이어 HP : ■■■■■■");
    }
    else if (MainCharacter.hp == 5) {
        printf("플레이어 HP : ■■■■■□");
    }
    else if (MainCharacter.hp == 4) {
        printf("플레이어 HP : ■■■■□□");
    }
    else if (MainCharacter.hp == 3) {
        printf("플레이어 HP : ■■■□□□");
    }
    else if (MainCharacter.hp == 2) {
        printf("플레이어 HP : ■■□□□□");
    }
    else if (MainCharacter.hp == 1) {
        printf("플레이어 HP : ■□□□□□");
    }
    else {
        printf("플레이어 HP : □□□□□□");
    }
}

void drawBombNumUI() {      // 폭탄 갯수를 그린다.
    SetCurrentCursorPos(STATUS_MENU_WINDOW_X, STATUS_MENU_WINDOW_Y + 2);
    if (MainCharacter.plusBombNumItem == 1) {
        printf("최대 물폭탄 개수 : ■□□□□□□□□□");
    }
    else if (MainCharacter.plusBombNumItem == 2) {
        printf("최대 물폭탄 개수 : ■■□□□□□□□□");
    }
    else if (MainCharacter.plusBombNumItem == 3) {
        printf("최대 물폭탄 개수 : ■■■□□□□□□□");
    }
    else if (MainCharacter.plusBombNumItem == 4) {
        printf("최대 물폭탄 개수 : ■■■■□□□□□□");
    }
    else if (MainCharacter.plusBombNumItem == 5) {
        printf("최대 물폭탄 개수 : ■■■■■□□□□□");
    }
    else if (MainCharacter.plusBombNumItem == 6) {
        printf("최대 물폭탄 개수 : ■■■■■■□□□□");
    }
    else if (MainCharacter.plusBombNumItem == 7) {
        printf("최대 물폭탄 개수 : ■■■■■■■□□□");
    }
    else if (MainCharacter.plusBombNumItem == 8) {
        printf("최대 물폭탄 개수 : ■■■■■■■■□□");
    }
    else if (MainCharacter.plusBombNumItem == 9) {
        printf("최대 물폭탄 개수 : ■■■■■■■■■□");
    }
    else {
        printf("최대 물폭탄 개수 : ■■■■■■■■■■");
    }
}

void drawPowerUI() {        // 화력의 크기를 그린다.
    SetCurrentCursorPos(STATUS_MENU_WINDOW_X, STATUS_MENU_WINDOW_Y + 4);
    if (MainCharacter.plusBombPowerItem == 2) {
        printf("물폭탄 화력 : ■□□□□□□□□□");
    }
    else if (MainCharacter.plusBombPowerItem == 3) {
        printf("물폭탄 화력 : ■■□□□□□□□□");
    }
    else if (MainCharacter.plusBombPowerItem == 4) {
        printf("물폭탄 화력 : ■■■□□□□□□□");
    }
    else if (MainCharacter.plusBombPowerItem == 5) {
        printf("물폭탄 화력 : ■■■■□□□□□□");
    }
    else if (MainCharacter.plusBombPowerItem == 6) {
        printf("물폭탄 화력 : ■■■■■□□□□□");
    }
    else if (MainCharacter.plusBombPowerItem == 7) {
        printf("물폭탄 화력 : ■■■■■■□□□□");
    }
    else if (MainCharacter.plusBombPowerItem == 8) {
        printf("물폭탄 화력 : ■■■■■■■□□□");
    }
    else if (MainCharacter.plusBombPowerItem == 9) {
        printf("물폭탄 화력 : ■■■■■■■■□□");
    }
    else if (MainCharacter.plusBombPowerItem == 10) {
        printf("물폭탄 화력 : ■■■■■■■■■□");
    }
    else {
        printf("물폭탄 화력 : ■■■■■■■■■■");
    }
}

void drawNpcHP() {      // NPC의 체력 즉 보스의 체력을 그린다.
    SetCurrentCursorPos(STATUS_MENU_WINDOW_X, STATUS_MENU_WINDOW_Y + 6);

    if (stageNum == 4) {
        if (bossCharacter.boss_hp == 10) {
            printf("보스 HP : ■■■■■■■■■■");
        }
        else if (bossCharacter.boss_hp == 9) {
            printf("보스 HP : ■■■■■■■■■□");
        }
        else if (bossCharacter.boss_hp == 8) {
            printf("보스 HP : ■■■■■■■■□□");
        }
        else if (bossCharacter.boss_hp == 7) {
            printf("보스 HP : ■■■■■■■□□□");
        }
        else if (bossCharacter.boss_hp == 6) {
            printf("보스 HP : ■■■■■■□□□□");
        }
        else if (bossCharacter.boss_hp == 5) {
            printf("보스 HP : ■■■■■□□□□□");
        }
        else if (bossCharacter.boss_hp == 4) {
            printf("보스 HP : ■■■■□□□□□□");
        }
        else if (bossCharacter.boss_hp == 3) {
            printf("보스 HP : ■■■□□□□□□□");
        }
        else if (bossCharacter.boss_hp == 2) {
            printf("보스 HP : ■■□□□□□□□□");
        }
        else if (bossCharacter.boss_hp == 1) {
            printf("보스 HP : ■□□□□□□□□□");
        }
        else {
            printf("보스 HP : □□□□□□□□□□");
        }
    }
    else {
        printf("%적의 수 : %d", countnpc());
    }
    SetCurrentCursorPos(STATUS_MENU_WINDOW_X, STATUS_MENU_WINDOW_Y + 8);
    printf(" %d Stage", stageNum);
}
int nextStage() {       // 다음 스테이지로 넘어가는 함수
    int i, j;

    stageNum++;
    if (stageNum == 5) {
        return 0;
    }
    if (stageNum == 4) { //만약 보스라고 생각한다.
        for (i = 0; i < GBOARD_HEIGHT + 2; i++) {
            for (j = 0; j < GBOARD_WIDTH + 2; j++) {
                gameBoardInfo[i][j] = gameBoardInfo4[i][j];
            }
        }
        setpc(1, 1); // pc 위치 초기화
        spawnnpc(1);
        MainCharacter.bombNum = 0;
        MainCharacter.hp = 3;
        MainCharacter.plusBombNumItem = 3; // 물폭탄 개수 초기화
        MainCharacter.plusBombPowerItem = 2; // 화력 초기화
        drawingTotalMap();
        npcspeed = 15;
        return 2; //return 2로 신호를 준다.
    }
    if (stageNum == 3) {
        for (i = 0; i < GBOARD_HEIGHT + 2; i++) {
            for (j = 0; j < GBOARD_WIDTH + 2; j++) {
                gameBoardInfo[i][j] = gameBoardInfo3[i][j];
            }
        }
        npcspeed = 2;
    }
    if (stageNum == 2) {
        for (i = 0; i < GBOARD_HEIGHT + 2; i++) {
            for (j = 0; j < GBOARD_WIDTH + 2; j++) {
                gameBoardInfo[i][j] = gameBoardInfo2[i][j];
            }
        }
        npcspeed = 5;
    }
    if (stageNum == 1) {
        for (i = 0; i < GBOARD_HEIGHT + 2; i++) {
            for (j = 0; j < GBOARD_WIDTH + 2; j++) {
                gameBoardInfo[i][j] = gameBoardInfo1[i][j];
            }
        }
    }

    setpc(1, 1); // pc 위치 초기화
    spawnnpc(4); //각각 4개의 npc를 소환하는 함수 만듬
    MainCharacter.bombNum = 0;
    MainCharacter.hp = 3;
    MainCharacter.plusBombNumItem = 3; // 물폭탄 개수 초기화
    MainCharacter.plusBombPowerItem = 2; // 화력 초기화
    drawingTotalMap();

    return 1;
}

void drawingGameMenu() {        // 게임 메뉴를 그린다.
    if (curMenu == 0) {
        SetCurrentCursorPos(50, 10);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), RED);
        printf("게임시작");
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
        SetCurrentCursorPos(50, 15);
        printf("게임종료");
    }
    if (curMenu == 1) {
        SetCurrentCursorPos(50, 10);
        printf("게임시작");
        SetCurrentCursorPos(50, 15);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), RED);
        printf("게임종료");
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
    }

}

void menuUp() {     // 게임 메뉴 위쪽 방향키 눌렀을때
    curMenu = 0;
}

void menuDown() {       // 게임 메뉴 아래 방향키 눌렀을때
    curMenu = 1;
}

void erazeWindow() {        // 콘솔창을 꺠끗하게 비워주는 함수
    int x, y;
    int cursX, cursY;

    for (y = 0; y < 25 + 2; y++)
    {
        for (x = 0; x < 100 + 2; x++)
        {
            cursX = x * 2;
            cursY = y;
            SetCurrentCursorPos(cursX, cursY);
            printf("  ");
        }
    }
}
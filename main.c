#pragma warning (disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <string.h>
#include <time.h>

typedef enum ColorType {
    BLACK,     //0
    darkBLUE,   //1
    DarkGreen,   //2
    darkSkyBlue,    //3
    DarkRed,     //4
    DarkPurple,   //5
    DarkYellow,   //6
    GRAY,      //7
    DarkGray,   //8
    BLUE,      //9
    GREEN,      //10
    SkyBlue,   //11
    RED,      //12
    PURPLE,      //13
    YELLOW,      //14
    WHITE      //15
}COLOR;
enum Block {
    BlockSolid = 100,
    BlockWeak = 101
}BLOCK;
enum Item {
    ItemHeart = 200,
    ItemBomb = 201,
    ItemPower = 202
} ITEM;
enum Bomb {
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
enum Pc {
    PcNormal = 400,
    PcOnBomb = 401
} PC;
enum Npc {
    NpcPattern = 500,
    NpcNOPattern = 501
} NPC;

typedef struct PCc {
    COORD pos;

}PC_pos;
typedef struct NPCc {
    COORD pos;
    BOOLEAN live;
}NPC_pos_pattern;
typedef struct NPCcc {
    COORD pos;
    BOOLEAN live;
}NPC_pos_nopattern;
typedef struct mainCharacterInfo {
    int hp;
    int bombNum;                                                                            //캐릭터가 놓은 Bomb의 갯수
    int plusBombNumItem;
    int plusBombPowerItem;

}mainCharacterInfo;
mainCharacterInfo MainCharacter;

#define KUP 72
#define KDOWN 80
#define KLEFT 75
#define KRIGHT 77
#define KSPACE 32

#define GBOARD_WIDTH 15
#define GBOARD_HEIGHT 15
#define GBOARD_ORIGIN_X 4
#define GBOARD_ORIGIN_Y 2
PC_pos* pc;
NPC_pos_pattern* npc_pattern;
NPC_pos_nopattern* npc_nopattern;
int cnt_npc_pattern;
int cnt_npc_nopattern;
void drawingTotalMap();


double TimeBoardInfo[GBOARD_HEIGHT + 2][GBOARD_WIDTH + 2];
//플레이어가 물풍선닿았을 때 색깔 바뀌는
int gameBoardInfo[GBOARD_HEIGHT + 2][GBOARD_WIDTH + 2] = {
    {100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100},
    {100,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,100},
    {100,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,202,0  ,0  ,0  ,0  ,0  ,0  ,100},
    {100,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,100},
    {100,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,100},
    {100,0  ,0  ,0  ,0  ,200,0  ,0  ,0  ,0  ,0  ,0  ,201,0  ,0  ,0  ,100},
    {100,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,100},
    {100,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,100},
    {100,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,201,0  ,0  ,0  ,100},
    {100,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,202,0  ,0  ,0  ,0  ,0  ,0  ,100},
    {100,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,100},
    {100,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,202,0  ,0  ,0  ,0  ,0  ,0  ,100},
    {100,0  ,0  ,0  ,0  ,0  ,200,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,100},
    {100,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,100},
    {100,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,100},
    {100,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,100},
    {100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100},
};

void firstTimeBoardInfo(double current_time, int x, int y)
{
    TimeBoardInfo[y][x] = current_time;


}
void putBomb(int x, int y)
{
    if (isBombNum(x, y) == 1)
    {

        gameBoardInfo[y][(x)] = 300;
        time_t start = time(NULL);
        double current_time = (double)start;
        firstTimeBoardInfo(current_time, x, y);
        MainCharacter.bombNum++;
    }
    else return;
}
int isBombNum(int x, int y)
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
int  isMiddleBomb(int i, int j)

{
    if (gameBoardInfo[i][j] >= BombFour && gameBoardInfo[i][j] <= BombOne2)
    {
        return 1;
    }
    else return 0;

}
int isFinalBomb(int i, int j)
{
    if (gameBoardInfo[i][j] == BombOne2)
    {
        return 1;
    }
    else return 0;
}
int isWaterLine(int i, int j)
{
    if (gameBoardInfo[i][j] == BombZero)
        return 1;
    else return 0;

}
int  detectCharacter(int i, int j)
{
    if (gameBoardInfo[i][j] == 400 || gameBoardInfo[i][j] == 401)
    {
        MainCharacter.hp--;
        printf("캐릭터hp:%d", MainCharacter.hp);
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
int detectBlock(int i, int j)
{
    if (gameBoardInfo[i][j] == BlockWeak) {


        return 0;
    }
    if (gameBoardInfo[i][j] == BlockSolid)return 1;



    return 0;
}
int makeRandomItem()
{
    int ran = rand() % 3;
    int itemN = 200 + ran;
    return itemN;
}
void explosion()
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

                    if ((detectCharacter(i, j + x) == 0 && (detectBlock(i, j + x) == 0) && gameBoardInfo[i][j + x] == 0) || gameBoardInfo[i][j + x] == BlockWeak)
                    {
                        if (gameBoardInfo[i][j + x] == BlockWeak)
                        {
                            int ran = rand() % 3;
                            if (ran == 0)
                            {
                                gameBoardInfo[i][j + x] = makeRandomItem();
                            }
                        }
                        if (gameBoardInfo[i][j + x]<ItemHeart || gameBoardInfo[i][j + x]>ItemPower)

                            gameBoardInfo[i][j + x] = BombZero;




                    }
                    else
                        break;
                }
                for (int x = 1; x < MainCharacter.plusBombPowerItem; x++)
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
                        }
                        if (gameBoardInfo[i][j - x]<ItemHeart || gameBoardInfo[i][j - x]>ItemPower)
                            gameBoardInfo[i][j - x] = BombZero;

                    }
                    else
                        break;
                }
                for (int x = 1; x < MainCharacter.plusBombPowerItem; x++)
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
                        }
                        if (gameBoardInfo[i + x][j]<ItemHeart || gameBoardInfo[i + x][j]>ItemPower)
                            gameBoardInfo[i + x][j] = BombZero;

                    }
                    else
                        break;
                }
                for (int x = 1; x < MainCharacter.plusBombPowerItem; x++)
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
                        }
                        if (gameBoardInfo[i - x][j]<ItemHeart || gameBoardInfo[i - x][j]>ItemPower)
                            gameBoardInfo[i - x][j] = BombZero;

                    }
                    else
                        break;
                }







            }
        }
    }
}

void findChangingBomb(double current_time)
{
    for (int i = 0; i < 17; i++)
    {
        for (int j = 0; j < 17; j++)
        {
            if (isMiddleBomb(i, j) == 1)
            {




                if (current_time - TimeBoardInfo[i][j] >= 7)
                {
                    gameBoardInfo[i][j] = BombOne2;
                }

                if (current_time - TimeBoardInfo[i][j] >= 6 && current_time - TimeBoardInfo[i][j] < 7)
                {

                    gameBoardInfo[i][j] = BombOne;
                }
                if (current_time - TimeBoardInfo[i][j] >= 5 && current_time - TimeBoardInfo[i][j] < 6)
                {

                    gameBoardInfo[i][j] = BombTwo2;
                }
                if (current_time - TimeBoardInfo[i][j] >= 4 && current_time - TimeBoardInfo[i][j] < 5)
                {

                    gameBoardInfo[i][j] = BombTwo;
                }
                if (current_time - TimeBoardInfo[i][j] >= 3 && current_time - TimeBoardInfo[i][j] < 4)
                {

                    gameBoardInfo[i][j] = BombThree2;
                }
                if (current_time - TimeBoardInfo[i][j] >= 2 && current_time - TimeBoardInfo[i][j] < 3)
                {

                    gameBoardInfo[i][j] = BombThree;
                }
                if (current_time - TimeBoardInfo[i][j] >= 1 && current_time - TimeBoardInfo[i][j] < 2)
                {

                    gameBoardInfo[i][j] = BombFour2;
                }




            }
        }
    }
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

//한강섭
int abs(int n) {
    if (n < 0) return n * -1;
    else return n;
}

void setpc(int y, int x) {
    pc->pos.Y = y;
    pc->pos.X = x;
    gameBoardInfo[y][x] = 400;
    return;
}
void setnpc_pattern(int i, int y, int x) {
    npc_pattern[i].live = TRUE;
    npc_pattern[i].pos.Y = y;
    npc_pattern[i].pos.X = x;
    gameBoardInfo[y][x] = 500;
    return;
}
void setnpc_nopattern(int i, int y, int x) {
    npc_pattern[i].live = TRUE;
    npc_nopattern[i].pos.Y = y;
    npc_nopattern[i].pos.X = x;
    gameBoardInfo[y][x] = 501;
    return;
}

int DetectpcCollision(int y, int x) {
    if (gameBoardInfo[y][x] == 0) return 0;//아무것도 없는 곳이니깐 움직인다 
    else if (gameBoardInfo[y][x] == 500 || gameBoardInfo[y][x] == 501) { //다음 위치가 npc랑 만나는 위치일때 
        MainCharacter.hp--;
        printf("캐릭터hp:%d", MainCharacter.hp);
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
        return 0; //pc를 다음위치로 움직인다.
    }
    else if (gameBoardInfo[y][x] == 200) {
        MainCharacter.hp += 1;
        printf("캐릭터hp:%d", MainCharacter.hp);
        gameBoardInfo[y][x] = 0;
        return 0;
    }
    else if (gameBoardInfo[y][x] == 201) {
        MainCharacter.plusBombNumItem += 1;
        gameBoardInfo[y][x] = 0;
        return 0;
    }
    else if (gameBoardInfo[y][x] == 202) {
        MainCharacter.plusBombPowerItem += 1;
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

void move_pc(int y, int x) {
    if (DetectpcCollision(pc->pos.Y + y, pc->pos.X + x)) return;
    gameBoardInfo[pc->pos.Y][pc->pos.X] = 0;
    pc->pos.X += x;
    pc->pos.Y += y;
    gameBoardInfo[pc->pos.Y][pc->pos.X] = 400;
    drawingTotalMap();
    return;
}
void move_pattern_npc() {
    for (int i = 0; i < cnt_npc_pattern; i++)
    {
        if (!npc_pattern[i].live) continue;
        int random;
        random = rand() % 4; //random 0 왼 1 오 2 위 3 아래
        int x, y;
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
        if (DetectnpcCollision(npc_pattern[i].pos.Y + y, npc_pattern[i].pos.X + x) == 1) continue; //막혔을때
        else if (DetectnpcCollision(npc_pattern[i].pos.Y + y, npc_pattern[i].pos.X + x) == 2) { //pc랑 부딪혔을때
            MainCharacter.hp--;
            printf("캐릭터hp:%d", MainCharacter.hp);
            gameBoardInfo[npc_pattern[i].pos.Y][npc_pattern[i].pos.X] = 0;
            npc_pattern[i].live = FALSE;
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
void move_nopattern_npc() {
    for (int i = 0; i < cnt_npc_nopattern; i++)
    {
        if (!npc_nopattern[i].live) continue; //살아있는 것만 작동시킨다. 
        int x, y;
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
        if (DetectnpcCollision(npc_nopattern[i].pos.Y + y, npc_nopattern[i].pos.X + x) == 1) continue; //막혔을때
        else if (DetectnpcCollision(npc_nopattern[i].pos.Y + y, npc_nopattern[i].pos.X + x) == 2) { //pc랑 부딪혔을때
            MainCharacter.hp--;
            printf("캐릭터hp:%d", MainCharacter.hp);
            gameBoardInfo[npc_nopattern[i].pos.Y][npc_nopattern[i].pos.X] = 0;
            npc_nopattern[i].live = FALSE;
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
BOOLEAN npc_alldiecheck() {
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
//나중에 pc체력이 0일때도 작성
//보스 공격 할 거 만들어 논거 
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
        time_t start = time(NULL);
        double current_time = (double)start;
        firstTimeBoardInfo(current_time, x, y);
    }
    return;
}



int before_key;
void ProcessKeyInput() {
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

        Sleep(20);
    }
    return;
}
int main() {
    srand(time(NULL));
    RemoveCursor();
    drawingTotalMap();
    Sleep(1000);
    pc = malloc(sizeof(PC_pos));
    setpc(3, 4);
    spawnnpc(2); //각각 3개의 npc를 소환하는 함수 만듬
    MainCharacter.bombNum = 0;
    MainCharacter.plusBombNumItem = 10;
    MainCharacter.plusBombPowerItem = 4;
    MainCharacter.hp = 3;
    drawingTotalMap();
    while (1) {
        //printf("%lf", time);
        time_t current_time = time(NULL);
        double time = (double)current_time;
        findChangingBomb(time);
        explosion();
        ProcessKeyInput();
        move_pattern_npc();
        move_nopattern_npc();
        if (npc_alldiecheck()) break; //npc가 모두 죽으면 끝내준다. 
    }
    return 0;
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
                default:
                    break;
                }
            }
            else if (500 <= gameBoardInfo[y][x] && gameBoardInfo[y][x] < 600) {
                switch (gameBoardInfo[y][x]) {
                case NpcNOPattern:
                    printf("§");
                    break;
                case NpcPattern:
                    printf("※");
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
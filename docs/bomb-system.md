# 시간 테이블 기반 물폭탄 시스템 구현

이 문서에서는 C 언어로 구현된 봄버맨 스타일 게임의 핵심 기능인 물폭탄 시스템에 대해 상세히 설명합니다.

## 핵심 개념: TimeBoardInfo 배열

물폭탄 시스템의 중심에는 `TimeBoardInfo` 2차원 배열이 있습니다:

```c
double TimeBoardInfo[GBOARD_HEIGHT + 2][GBOARD_WIDTH + 2];
```

이 배열은 게임 보드의 각 위치에 설치된 폭탄의 설치 시간을 저장하는 '시간 지도'입니다. 게임 보드와 동일한 크기를 가지고 있어, 공간 좌표를 시간 정보와 직접 매핑할 수 있습니다.

## 시간 추적 방식

### 1. 폭탄 설치 시 시간 기록

플레이어가 폭탄을 설치할 때, 현재 시스템 시간이 해당 위치의 `TimeBoardInfo` 배열에 기록됩니다:

```c
void putBomb(int x, int y) {
    if (isBombNum(x, y) == 1) {
        gameBoardInfo[y][(x)] = 300;  // 폭탄 초기 상태 (BombFour)
        
        // 현재 시스템 시간 획득 및 기록
        clock_t start = clock();
        double current_time = (double)start / CLOCKS_PER_SEC;
        firstTimeBoardInfo(current_time, x, y);
        
        MainCharacter.bombNum++;
    }
}

void firstTimeBoardInfo(double current_time, int x, int y) {
    TimeBoardInfo[y][x] = current_time;
}
```

이 방식의 장점:
- 별도의 객체 생성 없이 시간 정보 관리
- 배열 인덱싱으로 O(1) 시간에 접근 가능
- 게임 보드와 동일한 구조로 직관적인 매핑

### 2. 폭탄 상태 업데이트

게임 루프에서는 `findChangingBomb()` 함수가 호출되어 모든 폭탄의 상태를 현재 시간에 맞게 업데이트합니다:

```c
void findChangingBomb(double current_time) {
    for (int i = 0; i < 17; i++) {
        for (int j = 0; j < 17; j++) {
            if (isMiddleBomb(i, j) == 1) {
                // 경과 시간 계산
                double elapsed = current_time - TimeBoardInfo[i][j];
                
                // 시간에 따른 상태 변경
                if (elapsed >= 5.6) {
                    gameBoardInfo[i][j] = BombOne2;  // 폭발 직전
                }
                if (elapsed >= 4.2 && elapsed < 4.9) {
                    gameBoardInfo[i][j] = BombOne;
                }
                if (elapsed >= 3.5 && elapsed < 4.2) {
                    gameBoardInfo[i][j] = BombTwo2;
                }
                // 추가 상태 변경...
                
                drawingTotalMap();  // 변경된 상태 화면에 반영
            }
        }
    }
}
```

폭탄 상태는 다음 순서로 변화합니다:
1. `BombFour` (처음 설치)
2. `BombFour2`
3. `BombThree`
4. `BombThree2`
5. `BombTwo`
6. `BombTwo2`
7. `BombOne`
8. `BombOne2` (폭발 직전)
9. 폭발 (BombZero)

이 다단계 상태 변화는 플레이어에게 시각적인 피드백을 제공하며 전략적 결정을 내릴 시간을 줍니다.

### 3. 폭발 처리

폭탄이 최종 상태(`BombOne2`)에 도달하면, `explosion()` 함수가 폭발을 처리합니다:

```c
void explosion() {
    // 기존 물줄기 제거
    for (int i = 0; i < 17; i++) {
        for (int j = 0; j < 17; j++) {
            if (isWaterLine(i, j) == 1) {
                gameBoardInfo[i][j] = 0;
            }
        }
    }

    // 폭발할 폭탄 찾기
    for (int i = 0; i < 17; i++) {
        for (int j = 0; j < 17; j++) {
            if (isFinalBomb(i, j) == 1) {
                MainCharacter.bombNum--;
                gameBoardInfo[i][j] = BombZero;
                
                // 네 방향으로 폭발 확산
                // 동쪽 방향 예시:
                for (int x = 1; x < MainCharacter.plusBombPowerItem; x++) {
                    // 장애물 체크 및 물줄기 전파
                    if (detectBossWeakBlock(i, j + x) == 1) {
                        // 보스 약점 공격
                    }
                    else if ((detectCharacter(i, j + x) == 0 && 
                             (detectBlock(i, j + x) == 0) && 
                             gameBoardInfo[i][j + x] == 0) || 
                             gameBoardInfo[i][j + x] == BlockWeak) {
                        // 물줄기 전파 또는 약한 블록 파괴
                    }
                    else break;
                }
                // 서, 남, 북 방향도 유사하게 처리
            }
        }
    }
}
```

## 연쇄 폭발 메커니즘

물폭탄 시스템의 가장 흥미로운 특징 중 하나는 연쇄 폭발입니다. 이는 특별한 알고리즘 없이도 자연스럽게 구현됩니다:

1. 폭발의 물줄기가 다른 폭탄에 닿으면, 그 폭탄의 상태가 즉시 `BombOne2`(최종 상태)로 변경됩니다.
2. 다음 `explosion()` 함수 호출 시, 이 폭탄도 폭발하게 됩니다.
3. 이 과정이 반복되어 연속적인 폭발이 발생합니다.

이는 플레이어에게 전략적 깊이를 더하는 요소입니다. 물폭탄을 적절히 배치하면 연쇄 폭발로 더 넓은 영역에 영향을 줄 수 있습니다.

## 메모리 효율성

이 시스템의 주목할만한 특징은 메모리 효율성입니다:

- 폭탄 객체를 별도로 생성하지 않음
- 기존 게임 보드 구조를 활용
- 추가 메모리 오버헤드가 최소화됨

## 결론

`TimeBoardInfo` 배열을 활용한 물폭탄 시스템으로 C 언어의 기본 기능만으로도 복잡한 시간 기반 게임 메커니즘을 구현 하였습니다. 간결한 데이터 구조와 직관적인 알고리즘을 통해 풍부한 게임플레이 경험을 제공합니다.

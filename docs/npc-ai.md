# NPC AI 구현 상세

이 문서에서는 봄버맨 스타일 게임에 구현된 두 가지 유형의 NPC AI 시스템에 대해 상세히 설명합니다.

## 듀얼 AI 시스템 개요

게임에는 서로 다른 행동 패턴을 가진 두 종류의 NPC가 있습니다:

1. **패턴형 NPC** (빨간색 `※`) - 플레이어를 적극적으로 추적
2. **비패턴형 NPC** (노란색 `§`) - 반랜덤 이동과 플레이어 감지 기능

이 두 가지 유형은 상호 보완적인 AI 행동을 제공하여 게임의 난이도와 전략적 요소를 증가시킵니다.

## NPC 데이터 구조

각 NPC 유형은 고유한 데이터 구조를 가지고 있습니다:

```c
typedef struct NPCc {           // NPC1(패턴형)의 상태를 정의
    COORD pos;
    BOOLEAN live;
}NPC_pos_pattern;

typedef struct NPCcc {          // NPC2(비패턴형)의 상태를 정의
    COORD pos;
    BOOLEAN live;
}NPC_pos_nopattern;
```

각 구조체는 위치(`pos`)와 생존 상태(`live`) 정보를 포함합니다. 이 간단한 구조로도 복잡한 행동을 구현할 수 있습니다.

## 패턴형 NPC AI 알고리즘

패턴형 NPC는 더 높은 확률로 플레이어를 직접 추적합니다:

```c
void move_pattern_npc() {
    for (int i = 0; i < cnt_npc_pattern; i++) {
        if (!npc_pattern[i].live) continue;
        int random;
        int x, y;
        
        // 추적 VS 랜덤 이동 확률 결정
        random = rand() % 6; // 숫자를 높이면 더 강하게 플레이어를 추적
        
        if (random < 4) {
            // 랜덤 이동 (66% 확률)
            random = rand() % 4; // 0:왼쪽, 1:오른쪽, 2:위, 3:아래
            // 방향 결정...
        }
        else {
            // 플레이어 추적 (33% 확률)
            y = pc->pos.Y - npc_nopattern[i].pos.Y;
            x = pc->pos.X - npc_nopattern[i].pos.X;
            
            // 더 큰 거리 차이가 있는 축으로 먼저 이동
            if (abs(y) > abs(x)) {
                // y축 이동 방향 결정
            }
            else {
                // x축 이동 방향 결정
            }
        }
        
        // 충돌 감지 및 이동 처리
    }
}
```

### 핵심 알고리즘 요소:

1. **추적 확률**: `random = rand() % 6`으로 랜덤 이동과 추적 이동 사이의 비율 조정
2. **방향성 이동**: x축, y축 중 더 큰 거리를 먼저 좁히는 방식으로 추적
3. **장애물 회피**: 이동 전 `DetectnpcCollision()` 함수로 충돌 검사

## 비패턴형 NPC AI 알고리즘

비패턴형 NPC는 상대적으로 낮은 확률로 플레이어를 추적합니다:

```c
void move_nopattern_npc() {
    for (int i = 0; i < cnt_npc_nopattern; i++) {
        if (!npc_nopattern[i].live) continue;
        int random;
        int x, y;
        
        // 추적 VS 랜덤 이동 확률 결정
        random = rand() % 4; // 숫자를 높이면 더 강하게 플레이어를 추적
        
        if (random < 3) {
            // 랜덤 이동 (75% 확률)
            random = rand() % 4; // 0:왼쪽, 1:오른쪽, 2:위, 3:아래
            // 방향 결정...
        }
        else {
            // 플레이어 추적 (25% 확률)
            // 패턴형 NPC와 동일한 추적 로직
        }
        
        // 충돌 감지 및 이동 처리
    }
}
```

### 핵심 알고리즘 요소:

1. **낮은 추적 확률**: 약 25%의 확률로만 플레이어를 추적
2. **높은 무작위성**: 대부분의 시간 동안 무작위 방향으로 이동
3. **동일한 추적 알고리즘**: 추적 시에는 패턴형 NPC와 동일한 방향 선택 로직 사용

## 충돌 감지 시스템

두 NPC 유형 모두 동일한 충돌 감지 시스템을 사용합니다:

```c
int DetectnpcCollision(int y, int x) {
    if (gameBoardInfo[y][x] == 0) return 0; // 빈 공간이면 이동 가능
    else if (gameBoardInfo[y][x] == 400) return 2; // 플레이어와 충돌
    else return 1; // 그 외 장애물과 충돌
}
```

이 함수는 세 가지 결과를 반환합니다:
- `0`: 이동 가능
- `1`: 장애물과 충돌 (이동 불가)
- `2`: 플레이어와 충돌 (데미지 발생 및 NPC 삭제)

## 플레이어 상호작용

NPC가 플레이어와 충돌하면 다음과 같은 처리가 이루어집니다:

```c
// 플레이어와 충돌 시 처리 (move_pattern_npc 및 move_nopattern_npc 함수 내부)
else if (DetectnpcCollision(npc_pattern[i].pos.Y + y, npc_pattern[i].pos.X + x) == 2) { 
    MainCharacter.hp--; // 플레이어 체력 감소
    gameBoardInfo[npc_pattern[i].pos.Y][npc_pattern[i].pos.X] = 0; // NPC 위치 초기화
    npc_pattern[i].live = FALSE; // NPC 제거
    gameBoardInfo[pc->pos.Y][pc->pos.X] = 402; // 플레이어 상태 변경 (피격 상태)
}
```

## 보스 전투 단계의 AI

보스 단계(Stage 4)에서는 기본 NPC AI에 추가적인 난이도 증가 요소가 적용됩니다:

```c
// 보스 단계 AI 로직 (ProcessKeyInput 함수 내부)
if (flag == 2) { // 보스 단계일 때
    if (e % 300 == 0) {
        addspawnnpc(2); // 새로운 NPC 추가 생성
    }
    if (e % 5000 == 0) {
        npcspeedup(); // NPC 이동 속도 증가
    }
    // 기타 보스 패턴...
}
```

보스 전투 중 주기적으로:
1. 새로운 NPC가 스폰됨
2. 기존 NPC의 이동 속도가 증가
3. 보스의 페이즈가 변화

## 난이도 조절 시스템

게임은 여러 메커니즘을 통해 NPC의 난이도를 조절합니다:

1. **NPC 속도 조절**: `npcspeed` 변수로 NPC 이동 속도 통제
```c
void npcspeedup() {
    if (npcspeed <= 0) return;
    npcspeed--;
}
```

2. **스테이지별 초기 속도 설정**:
```c
// nextStage 함수 내부
if (stageNum == 3) {
    // 스테이지 3 설정
    npcspeed = 2; // 빠른 속도
}
if (stageNum == 2) {
    // 스테이지 2 설정
    npcspeed = 5; // 중간 속도
}
```

3. **NPC 수 조절**: 스테이지와 보스 페이즈에 따라 NPC 수 증가

## 결론

C언러를 이용해 간단한 구조로 다양한 행동 패턴과 난이도 조절을 구현하였습니다.

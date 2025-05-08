# 콘솔 게임 렌더링과 React의 비교 분석

이 문서에서는 C 언어로 구현된 봄버맨 스타일 게임의 렌더링 시스템을 분석하고, 이를 현대적인 UI 라이브러리인 React의 렌더링 패러다임과 비교합니다.

## 콘솔 게임의 렌더링 구조

게임의 렌더링 시스템은 다음과 같은 핵심 함수들로 구성되어 있습니다:

```c
void drawingTotalMap() {
    printGameBoard();
    printHeroHp();
    drawBombNumUI();
    drawPowerUI();
    drawNpcHP();
}
```

이 함수는 게임 화면을 구성하는 여러 UI 요소들을 각각의 하위 함수로 분리하여 관리합니다. 각 함수는 특정 UI 요소에 대한 렌더링을 담당합니다:

```c
void printGameBoard() {
    // 게임 보드 그리기
}

void printHeroHp() {
    // 플레이어 HP 바 그리기
}

void drawBombNumUI() {
    // 폭탄 개수 UI 그리기
}

// 기타 UI 요소들...
```

## React 렌더링 패러다임과의 유사점

### 1. 컴포넌트 기반 구조

C 게임의 렌더링 함수들은 React의 컴포넌트와 개념적으로 유사합니다:

```jsx
// React 컴포넌트 구조로 표현
function GameScreen() {
  return (
    <>
      <GameBoard />
      <StatusPanel>
        <HeroHP />
        <BombNumUI />
        <PowerUI />
        <NpcHP />
      </StatusPanel>
    </>
  );
}
```

각 렌더링 함수는 특정 UI 부분을 담당하며, 이는 React의 단일 책임 원칙을 따르는 컴포넌트 설계와 유사합니다.

### 2. 상태 기반 렌더링

게임은 전역 변수들(`gameBoardInfo`, `MainCharacter` 등)에 상태를 저장하고, 이 상태가 변경될 때마다 화면을 다시 그립니다:

```c
void move_pc(int y, int x) {
    // 상태 변경
    gameBoardInfo[pc->pos.Y][pc->pos.X] = 0;
    pc->pos.X += x;
    pc->pos.Y += y;
    gameBoardInfo[pc->pos.Y][pc->pos.X] = 400;
    
    // 화면 다시 그리기
    drawingTotalMap();
}
```

이는 React의 상태 변경 시 리렌더링되는 모델과 유사합니다:

```jsx
function movePlayer(direction) {
  // 상태 업데이트
  setPlayerPosition(prev => ({
    x: prev.x + direction.x,
    y: prev.y + direction.y
  }));
  
  // React는 자동으로 리렌더링
}
```

### 3. 조건부 렌더링

게임에서는 조건에 따라 다른 내용을 렌더링하는 패턴이 자주 사용됩니다:

```c
void drawNpcHP() {
    SetCurrentCursorPos(STATUS_MENU_WINDOW_X, STATUS_MENU_WINDOW_Y + 6);

    if (stageNum == 4) {
        // 보스 HP 그리기
    }
    else {
        // 일반 적 수 표시
        printf("%적의 수 : %d", countnpc());
    }
}
```

이는 React의 조건부 렌더링과 유사합니다:

```jsx
function EnemyStatus({ stage, bossHP, enemyCount }) {
  return (
    <div>
      {stage === 4 ? (
        <BossHealthBar hp={bossHP} />
      ) : (
        <div>적의 수: {enemyCount}</div>
      )}
    </div>
  );
}
```

## 주요 차이점

### 1. 명령형 vs 선언형 프로그래밍

가장 큰 차이점은 프로그래밍 패러다임입니다:

- **C 게임(명령형)**: 커서 위치 지정, 문자 출력, 색상 변경 등 **어떻게** 화면을 그릴지 단계별로 지시합니다.

```c
void printHeroHp() {
    SetCurrentCursorPos(STATUS_MENU_WINDOW_X, STATUS_MENU_WINDOW_Y);
    if (MainCharacter.hp == 6) {
        printf("플레이어 HP : ■■■■■■");
    }
    else if (MainCharacter.hp == 5) {
        printf("플레이어 HP : ■■■■■□");
    }
    // 추가 조건...
}
```

- **React(선언형)**: 화면이 **무엇**처럼 보여야 하는지 선언하고, 실제 DOM 조작은 React가 담당합니다.

```jsx
function HeroHP({ hp, maxHP = 6 }) {
  return (
    <div>
      플레이어 HP: {[...Array(maxHP)].map((_, i) => 
        <span key={i}>{i < hp ? '■' : '□'}</span>
      )}
    </div>
  );
}
```

### 2. 렌더링 최적화

- **C 게임**: 상태가 변경될 때마다 관련된 부분만 업데이트하는 대신, 전체 화면을 다시 그립니다.
- **React**: 가상 DOM을 사용해 변경된 부분만 실제 DOM에 효율적으로 업데이트합니다.

### 3. 이벤트 처리

- **C 게임**: 폴링 방식으로 키보드 입력을 주기적으로 확인합니다.
```c
if (_kbhit() != 0) {
    key = _getch();
    // 키 처리...
}
```

- **React**: 이벤트 리스너를 통한 비동기 이벤트 처리 방식을 사용합니다.
```jsx
useEffect(() => {
  const handleKeyDown = (e) => {
    // 키 처리...
  };
  
  window.addEventListener('keydown', handleKeyDown);
  return () => window.removeEventListener('keydown', handleKeyDown);
}, []);
```

## TimeBoardInfo와 파생 상태

게임의 `TimeBoardInfo` 배열을 활용한 물폭탄 상태 관리는 React의 파생 상태(derived state) 개념과 유사합니다:

```c
void findChangingBomb(double current_time) {
    // 현재 시간과 저장된 시간을 비교하여 폭탄 상태 결정
}
```

React에서는 이러한 파생 상태를 `useMemo`나 selector 패턴으로 구현할 수 있습니다:

```jsx
const bombStates = useMemo(() => {
  return bombs.map(bomb => {
    const elapsed = currentTime - bomb.placedTime;
    // 경과 시간에 따른 상태 반환
  });
}, [bombs, currentTime]);
```

## 결론

이 C 콘솔 게임의 렌더링 시스템은 기본적인 구조와 개념 면에서 React와 같은 현대적 UI 라이브러리와 여러 유사점을 가지고 있습니다. 컴포넌트 분리, 상태 기반 렌더링, 조건부 렌더링 등의 패턴은 시대와 기술 스택을 뛰어넘는 UI 개발의 기본 원칙임을 보여줍니다.

물론 명령형 vs 선언형 프로그래밍, 렌더링 최적화, 이벤트 처리 방식 등에서 차이가 있지만, 기본적인 UI 구성 원리는 유사합니다.

이러한 비교를 통해 프론트엔드 개발이 어떻게 발전해왔는지, 그리고 어떤 기본 원칙들이 변함없이 중요한지 이해할 수 있습니다.

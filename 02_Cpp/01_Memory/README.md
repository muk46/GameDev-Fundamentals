# 수업 7-1: 스택 vs 힙 메모리 구조

> 날짜: 2026-04-06 | 2단계 C++ 보강 시작

---

## 프로그램 메모리 구조

```
┌──────────────────┐  높은 주소
│      스택 (Stack) │  ← 지역변수, 함수 호출 (자동, 빠름)
│        ↓         │
│        ↑         │
│      힙 (Heap)    │  ← new/malloc (수동, 느림)
├──────────────────┤
│   데이터 (Data)   │  ← 전역변수, static
├──────────────────┤
│   코드 (Code)     │  ← 실행 명령어
└──────────────────┘  낮은 주소
```

---

## 스택 (Stack)

```cpp
void Attack()
{
    int damage = 50;       // 스택에 할당
    float range = 3.5f;    // 스택에 할당
    // 함수 끝나면 자동 해제
}
```

- 함수 호출 시 자동 할당, 끝나면 자동 해제
- LIFO 구조
- 매우 빠름 (포인터만 이동)
- 크기 제한 있음 (보통 1~8MB)
- 지역변수, 매개변수, 리턴 주소 저장

---

## 힙 (Heap)

```cpp
void SpawnMonster()
{
    Monster* mob = new Monster();   // 힙에 할당
    mob->SetHP(100);
    delete mob;                     // 직접 해제
}
```

- new/delete로 수동 할당/해제
- 크기 제한 거의 없음
- 스택보다 느림 (빈 공간 찾아야 함)
- 해제 안 하면 메모리 누수
- 함수가 끝나도 살아있음

---

## 스택 vs 힙 비교

| | 스택 | 힙 |
|---|------|-----|
| 할당/해제 | 자동 | 수동 (new/delete) |
| 속도 | 빠름 | 느림 |
| 크기 | 제한적 (1~8MB) | 거의 무제한 |
| 수명 | 함수 끝나면 해제 | 직접 해제할 때까지 유지 |
| 용도 | 지역변수, 임시 데이터 | 게임 오브젝트, 큰 데이터 |

---

## 게임에서의 사용

```cpp
void GameLoop()
{
    // 스택: 임시 계산용
    float deltaTime = 0.016f;
    FVector direction(1, 0, 0);

    // 힙: 게임 오브젝트 (수명이 길어야 함)
    ACharacter* player = new ACharacter();
    AMonster* boss = new AMonster();

    delete player;
    delete boss;
}
```

게임 오브젝트가 힙에 있는 이유:
1. 함수가 끝나도 살아있어야 함
2. 크기가 클 수 있음
3. 수명을 자유롭게 제어해야 함

---

## 위험한 실수들

### 메모리 누수

```cpp
Monster* mob = new Monster();
// delete 안 함 → 포인터는 사라지지만 힙 객체는 남아있음
```

### 댕글링 포인터

```cpp
Monster* mob = new Monster();
delete mob;
mob->Attack();  // 해제된 메모리 접근 → 크래시!
```

### 스택 오버플로우

```cpp
void Recursive()
{
    int bigArray[100000];
    Recursive();  // 무한 재귀 → 스택 오버플로우!
}
```

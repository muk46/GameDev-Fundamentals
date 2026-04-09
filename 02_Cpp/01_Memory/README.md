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

---

# 수업 7-2: new/delete와 메모리 누수

> 날짜: 2026-04-07

## new와 delete

```cpp
// 단일 객체
Monster* mob = new Monster();    // 힙에 할당
delete mob;                       // 해제

// 배열
int* scores = new int[100];      // 배열 할당
delete[] scores;                  // 배열 해제 ([] 필수!)
```

### new가 하는 일

```
1. 힙에서 메모리 확보
2. 생성자 호출
```

### delete가 하는 일

```
1. 소멸자 호출
2. 메모리 반환
```

### 짝 맞추기 규칙

```
new       → delete
new[]     → delete[]
절대 섞지 말 것!
```

---

## 메모리 누수 패턴 5가지

### 1. 단순 해제 누락

```cpp
void SpawnEnemy()
{
    Monster* mob = new Monster();
    // delete 깜빡함 → 누수!
}
```

### 2. 조기 return으로 누락

```cpp
void LoadAsset(const char* path)
{
    Texture* tex = new Texture();
    if (!FileExists(path))
        return;         // delete 안 됨!
    delete tex;
}
```

### 3. 예외 발생으로 누락

```cpp
void Process()
{
    Data* data = new Data();
    DoSomething();      // 예외 발생 시
    delete data;        // 실행 안 됨 → 누수!
}
```

### 4. 포인터 덮어쓰기

```cpp
Monster* mob = new Monster();    // 첫 번째
mob = new Monster();             // 덮어씀 → 첫 번째 주소 상실 → 누수!
delete mob;                      // 두 번째만 해제
```

### 5. 컨테이너 안의 포인터

```cpp
std::vector<Monster*> monsters;
monsters.push_back(new Monster());
monsters.push_back(new Monster());

monsters.clear();  // 포인터만 지움, 힙 객체 누수!

// 올바른 방법:
for (Monster* m : monsters)
    delete m;
monsters.clear();
```

---

## 게임에서의 증상

```
누수가 쌓이면:
프레임 1:     [====      ] 500MB
프레임 50000: [==========] 메모리 부족 → 크래시!

"오래 하면 점점 느려지다가 튕김" = 메모리 누수 의심
```

## 올바른 습관

```
1. new 했으면 어디서 delete 할지 바로 결정
2. 조기 return 전에 해제 확인
3. 배열은 반드시 delete[]
4. 포인터 덮어쓰기 전에 기존 것 delete
5. → 스마트 포인터로 자동화 (다음 수업)
```

---

# 수업 7-3: unique_ptr

> 날짜: 2026-04-08

## 스마트 포인터란?

스코프({})를 벗어나면 자동 delete. new/delete 수동 관리의 누수 문제를 해결.

## unique_ptr — 단독 소유

```cpp
#include <memory>

// 기존 방식 (위험)
Monster* mob = new Monster();
delete mob;

// unique_ptr (안전)
std::unique_ptr<Monster> mob = std::make_unique<Monster>();
// 스코프 끝나면 자동 delete
```

### 자동 해제 원리

```
unique_ptr은 스택에 있는 객체
→ 스코프 끝나면 소멸자 호출
→ 소멸자에서 내부 포인터를 delete
```

조기 return, 예외 발생 시에도 안전하게 해제됨.

### 핵심 규칙: 복사 불가, 이동만 가능

```cpp
auto mob1 = std::make_unique<Monster>();

auto mob2 = mob1;              // 컴파일 에러! (복사 불가)
auto mob2 = std::move(mob1);   // OK! (소유권 이전)
// mob1은 이제 nullptr
```

복사를 막는 이유: 두 포인터가 같은 객체를 소유하면 이중 해제 → 크래시

### 주요 함수

```cpp
auto mob = std::make_unique<Monster>();

mob.get()           // 원시 포인터 얻기 (소유권 유지)
mob.release()       // 소유권 포기, 원시 포인터 반환
mob.reset(new_ptr)  // 기존 해제 후 새 객체로 교체
if (mob)            // nullptr 체크
```

### 함수에 전달

```cpp
// 소유권 넘기기
void TakeOwnership(std::unique_ptr<Monster> mob);
TakeOwnership(std::move(mob));

// 빌려주기 (소유권 유지)
void UseMonster(Monster* mob);
UseMonster(mob.get());
```

### new/delete vs unique_ptr

| | new/delete | unique_ptr |
|---|-----------|------------|
| 해제 | 수동 | 자동 |
| 누수 위험 | 높음 | 없음 |
| 복사 | 가능 (위험) | 불가 (안전) |
| 소유권 | 불명확 | 명확 (단독) |
| 성능 | 동일 | 동일 (오버헤드 0) |

---

# 수업 7-4: shared_ptr, weak_ptr, 순환 참조

> 날짜: 2026-04-09

## shared_ptr — 공유 소유

여러 곳에서 하나의 객체를 공유. 내부 참조 카운트로 관리.

```cpp
auto mob1 = std::make_shared<Monster>();  // 카운트: 1
auto mob2 = mob1;                          // 카운트: 2
auto mob3 = mob1;                          // 카운트: 3

// mob3 소멸 → 2, mob2 소멸 → 1, mob1 소멸 → 0 → delete!
```

### unique_ptr vs shared_ptr

| | unique_ptr | shared_ptr |
|---|-----------|------------|
| 소유자 | 1명 | 여러 명 |
| 복사 | 불가 | 가능 |
| 성능 | 오버헤드 0 | 참조 카운트 오버헤드 |
| 원칙 | **기본 선택** | 공유 필요할 때만 |

---

## 순환 참조 문제

```cpp
class Player { std::shared_ptr<Monster> target; };
class Monster { std::shared_ptr<Player> aggro; };

player->target = monster;   // M 카운트: 2
monster->aggro = player;    // P 카운트: 2

// 함수 끝나도 카운트가 1로만 내려감 → 영원히 해제 안 됨!
```

```
Player ──shared_ptr──→ Monster
   ↑                      │
   └──────shared_ptr──────┘
서로 잡고 있으면 카운트가 0이 될 수 없다 → 메모리 누수
```

---

## weak_ptr — 순환 참조 해결

참조 카운트를 증가시키지 않는 포인터. 소유하지 않고 관찰만.

```cpp
class Player { std::shared_ptr<Monster> target; };   // 강한 참조
class Monster { std::weak_ptr<Player> aggro; };       // 약한 참조

// weak_ptr은 카운트 안 올림 → 순환 참조 해결!
```

### weak_ptr 사용법

```cpp
std::weak_ptr<Player> wp = sharedPlayer;

// 직접 사용 불가, lock()으로 변환
if (auto sp = wp.lock())    // 살아있으면 shared_ptr 반환
{
    sp->Attack();
}
else
{
    // 이미 해제됨
}

wp.expired()   // 객체가 삭제되었는지 확인
```

---

## 스마트 포인터 총정리

| | unique_ptr | shared_ptr | weak_ptr |
|---|-----------|------------|----------|
| 소유 | 단독 | 공유 | 소유 안 함 |
| 복사 | 불가 | 가능 | 가능 |
| 카운트 증가 | - | O | X |
| 직접 사용 | O | O | X (lock 필요) |
| 용도 | 기본 선택 | 공유 필요 시 | 순환 참조 방지 |

### 선택 기준

```
혼자 소유?           → unique_ptr
여러 곳에서 공유?    → shared_ptr
참조만, 소유 안 함?  → weak_ptr
순환 참조 위험?      → 한쪽을 weak_ptr로
```

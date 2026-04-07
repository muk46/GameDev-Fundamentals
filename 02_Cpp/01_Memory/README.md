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

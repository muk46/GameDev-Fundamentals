# 수업 8-1: 가상 함수와 vtable

> 날짜: 2026-04-13 | 수업 8 다형성 심화 시작

---

## 다형성이란?

하나의 인터페이스로 여러 타입을 다루는 것. 부모 포인터로 자식 객체들을 똑같이 다룰 수 있게 해준다.

---

## virtual 없을 때 (정적 바인딩)

```cpp
class Monster
{
public:
    void Attack() { std::cout << "몬스터 공격\n"; }
};

class Dragon : public Monster
{
public:
    void Attack() { std::cout << "드래곤 브레스!\n"; }
};

Monster* mob = new Dragon();
mob->Attack();   // "몬스터 공격" ← 부모 함수!
```

컴파일러는 포인터 타입(`Monster*`)만 보고 `Monster::Attack`을 호출. 런타임에 실제 객체가 Dragon인지 모른다.

---

## virtual 있을 때 (동적 바인딩)

```cpp
class Monster
{
public:
    virtual void Attack() { std::cout << "몬스터 공격\n"; }
};

class Dragon : public Monster
{
public:
    void Attack() override { std::cout << "드래곤 브레스!\n"; }
};

Monster* mob = new Dragon();
mob->Attack();   // "드래곤 브레스!" ← 실제 객체 기준!
```

```
정적 바인딩 (virtual 없음): 컴파일 타임에 결정 → 포인터 타입 기준
동적 바인딩 (virtual 있음): 런타임에 결정     → 실제 객체 기준
```

---

## vtable (가상 함수 테이블)

클래스마다 가상 함수들의 주소를 담은 테이블.

```
Monster의 vtable:
[0] Monster::Attack
[1] Monster::Die

Dragon의 vtable:
[0] Dragon::Attack     ← override됨
[1] Monster::Die       ← 그대로
```

### vptr (vtable 포인터)

가상 함수가 있는 클래스의 각 객체는 자신의 vtable을 가리키는 숨은 포인터 `vptr`을 가진다.

```
Dragon 객체:
┌──────────────────┐
│ vptr  (8 bytes)  │ → Dragon의 vtable
│ m_HP             │
│ m_Name           │
└──────────────────┘
```

### 호출 과정

```cpp
mob->Attack();
```

```
1. mob의 객체에서 vptr 읽기
2. vptr이 가리키는 vtable에서 Attack 주소 찾기
3. 그 주소의 함수 호출
```

---

## 가상 소멸자 (매우 중요)

virtual 없는 소멸자 → 자식 소멸자 호출 안 됨 → 메모리 누수!

```cpp
class Monster
{
public:
    virtual ~Monster() { }   // ← virtual 필수!
};

Monster* mob = new Dragon();
delete mob;
// Dragon 소멸 → Monster 소멸 (정상)
```

**규칙: 상속을 사용할 기반 클래스의 소멸자는 반드시 virtual.**

---

## 가상 함수의 비용

```
메모리: 클래스당 vtable 1개, 객체당 vptr 1개 (8 bytes)
속도:   포인터 간접 접근 2번 (수 나노초)
인라인: 불가능
```

게임에서 매 프레임 호출되는 `Update()` 같은 함수는 누적 비용 고려. 하지만 대부분 무시 가능한 수준.

---

## 핵심 정리

| | virtual 없음 | virtual 있음 |
|---|------|------|
| 바인딩 | 정적 (컴파일 타임) | 동적 (런타임) |
| 기준 | 포인터 타입 | 실제 객체 |
| 구현 | 직접 호출 | vtable → vptr |
| 비용 | 0 | 아주 약간 |

---

## 퀴즈 결과 (3문제)

**2 / 3**

| Q | 주제 | 결과 |
|---|------|------|
| Q1 | virtual 없는 함수의 호출 결과 | **X** |
| Q2 | vtable의 내용 | O |
| Q3 | 가상 소멸자 빠뜨린 결과 | O |

### 헷갈린 포인트

**Q1 착각**: "Dragon 객체를 new 했으니 Dragon::Attack 호출된다" → **틀림!**
→ virtual이 없으면 **포인터 타입** 기준. `Monster*` 니까 `Monster::Attack`.
→ virtual이 있어야 실제 객체(Dragon)를 본다.

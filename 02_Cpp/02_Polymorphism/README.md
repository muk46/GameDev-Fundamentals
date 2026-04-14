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

---

# 수업 8-2: override와 final

> 날짜: 2026-04-14

## 왜 override가 필요한가?

시그니처 다르면 override가 아니라 **오버로드**가 되는 함정.

```cpp
class Monster
{
public:
    virtual void Attack(int d) { }
};

class Dragon : public Monster
{
public:
    void Attack(float d) { }   // int가 아닌 float → 새 함수!
};

Monster* mob = new Dragon();
mob->Attack(10);   // Monster::Attack 호출! (override 안 됨)
```

컴파일러는 에러로 보지 않고 정상 처리 → 버그 찾기 힘듦.

---

## override 키워드

```cpp
class Dragon : public Monster
{
public:
    void Attack(float d) override { }   // 컴파일 에러!
    // "override할 가상 함수를 찾을 수 없습니다"
};
```

컴파일러가 부모에 같은 시그니처의 가상 함수가 있는지 검증한다.

### override의 3가지 이점

1. **오타 방지**: `Attak()` → 컴파일 에러
2. **시그니처 불일치 방지**: `float` vs `int` → 컴파일 에러
3. **가독성**: "이건 override다" 명시

**규칙: 가상 함수를 재정의할 때는 항상 `override`.**

---

## final 키워드

### 함수에 final → 더 이상 override 금지

```cpp
class Dragon : public Monster
{
public:
    void Attack() override final { }
};

class FireDragon : public Dragon
{
public:
    void Attack() override { }   // 컴파일 에러!
};
```

### 클래스에 final → 상속 자체 금지

```cpp
class Dragon final : public Monster { };

class FireDragon : public Dragon { };   // 컴파일 에러!
```

---

## 언제 final을 쓰나?

```
1. 성능: 컴파일러가 가상 호출을 직접 호출로 최적화 (vtable 조회 스킵)
2. 설계: "이 이상 확장 금지" 명시
3. 안전: 실수로 상속/override되는 것 방지
```

---

## 키워드 총정리

| 키워드 | 위치 | 의미 |
|--------|------|------|
| `virtual` | 부모 함수 | 이 함수는 override 가능 |
| `override` | 자식 함수 | 부모 가상 함수 재정의 (검증) |
| `final` | 자식 함수 | 더 이상 override 금지 |
| `final` | 클래스 | 상속 금지 |

```
virtual  → 문 열기
override → "제대로 들어왔는지 확인해줘"
final    → 문 닫기
```

---

## 퀴즈 결과 (3문제)

**2 / 3**

| Q | 주제 | 결과 |
|---|------|------|
| Q1 | 시그니처 다른 "override"의 결과 | O |
| Q2 | override를 쓰는 이유 | O |
| Q3 | 클래스 final의 효과 | **X** |

### 헷갈린 포인트

**Q3**: 함수 final과 클래스 final 구분
- 함수 final → **override 금지**
- 클래스 final → **상속 금지**

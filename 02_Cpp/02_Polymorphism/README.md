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

---

# 수업 8-3: 순수 가상 함수와 추상 클래스

> 날짜: 2026-04-15

## 문제 상황

게임에서 모든 무기는 `Fire()`를 반드시 구현해야 한다. virtual만 쓰면 자식이 까먹어도 컴파일됨 → 런타임에 부모 빈 함수 호출되는 버그.

---

## 순수 가상 함수 (Pure Virtual)

```cpp
class Weapon
{
public:
    virtual void Fire() = 0;   // "= 0" 이 순수 가상
};
```

의미: **"구현 없음. 자식이 반드시 override 해야 함."**

---

## 추상 클래스 (Abstract Class)

순수 가상 함수를 **하나라도** 가진 클래스 = 추상 클래스.

```cpp
Weapon w;                  // 컴파일 에러! 인스턴스화 불가
Weapon* w = new Sword();   // OK (Sword가 Fire 구현했다면)
```

### 자식이 override 안 하면?

```cpp
class Sword : public Weapon { };   // Fire() 구현 안 함
Sword s;   // 컴파일 에러! Sword도 여전히 추상 클래스
```

→ 순수 가상을 override 안 한 자식도 추상 클래스가 된다.

---

## 올바른 사용

```cpp
class Weapon
{
public:
    virtual ~Weapon() = default;
    virtual void Fire() = 0;
    virtual int GetDamage() const = 0;
};

class Sword : public Weapon
{
public:
    void Fire() override { std::cout << "검 휘두르기\n"; }
    int GetDamage() const override { return 20; }
};

std::vector<Weapon*> weapons;
weapons.push_back(new Sword());
weapons.push_back(new Bow());

for (auto* w : weapons)
    w->Fire();   // 다형성: 각자의 구현 호출
```

---

## 인터페이스 패턴

모든 함수가 순수 가상 → 인터페이스 역할.

```cpp
class IDamageable
{
public:
    virtual ~IDamageable() = default;
    virtual void TakeDamage(int amount) = 0;
    virtual bool IsDead() const = 0;
};

class Player : public IDamageable { /* 구현 */ };
class Monster : public IDamageable { /* 구현 */ };
```

"피해 받을 수 있는 모든 것"을 하나로 다룰 수 있다.

---

## 순수 가상도 구현 가능 (덜 알려진 기능)

```cpp
class Weapon
{
public:
    virtual void Fire() = 0;
};

void Weapon::Fire() { std::cout << "기본 발사음\n"; }

class Sword : public Weapon
{
public:
    void Fire() override
    {
        Weapon::Fire();   // 부모 기본 동작 호출
        std::cout << "검 휘두르기\n";
    }
};
```

"반드시 override 해라. 하지만 기본 동작은 제공해줄게."

---

## 정리

| 구분 | virtual | 순수 가상 (= 0) |
|------|---------|-----------------|
| 구현 제공 | 필수 | 선택 |
| 자식 override | 선택 | **강제** |
| 인스턴스화 | 가능 | **불가** |
| 용도 | 재정의 허용 | 인터페이스 강제 |

```
virtual  → "override 해도 된다"
= 0      → "override 해야만 한다"
```

---

## 퀴즈 결과 (3문제)

**1 / 3**

| Q | 주제 | 결과 |
|---|------|------|
| Q1 | 추상 클래스 인스턴스화 시도 | O |
| Q2 | 추상 클래스 판별 (4개 중) | **X** |
| Q3 | 순수 가상 vs 일반 가상 선택 | **X** |

### 헷갈린 포인트

**Q2 착각**: A, D를 추상 클래스로 선택 → 완전히 반대
- 추상 클래스 = 순수 가상 함수(`= 0`)를 **하나라도** 가진 클래스
- A: `virtual void F() { }` → 구현 있음 → 추상 아님
- B: `virtual void F() = 0;` → 순수 가상 → **추상**
- C: B 상속 후 F() override 안 함 → 순수 가상 남아있음 → **추상**
- D: F() override로 구현 제공 → 추상 아님
- **핵심**: 자식이 override 안 하면 자식도 여전히 추상 클래스

**Q3 착각**: "강제 구현"과 "기본 동작"의 선언을 반대로 선택
- 반드시 구현 강제 → **`= 0`** (순수 가상)
- 기본 동작 제공, override는 선택 → **일반 virtual**
- `= 0`의 의미를 재확인: "구현 없음, 자식이 만들어야 함"
- 기본 동작을 주고 싶으면 `= 0`을 쓰면 안 됨

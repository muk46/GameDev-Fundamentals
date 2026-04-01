# 쿼터니언 (Quaternion)

## 오일러 각 (Euler Angles)

3D 회전을 X, Y, Z 축 각각의 회전 각도로 표현.

```
X축 회전 (Pitch):  고개 끄덕이기  ↕
Y축 회전 (Yaw):    좌우 둘러보기  ↔
Z축 회전 (Roll):   고개 갸우뚱    ↻

UE:    FRotator(Pitch, Yaw, Roll)
Unity: transform.eulerAngles
```

직관적이지만 치명적인 문제가 있다.

## 짐벌락 (Gimbal Lock)

특정 축이 90° 회전하면 두 축이 겹쳐서 한 방향의 회전이 사라지는 현상.

```
정상 상태:              짐벌락 발생 (Pitch 90°):

    Y ↑                     Y ↑
    |                       |
    *──→ X                  *──→ X, Z 겹침!
   /
  ↙ Z                      Z축 회전이 사라짐!

3개 축으로 회전 가능       2개 축으로만 회전 가능 (1자유도 손실)
```

### 실제 예시

FPS 게임에서 하늘을 정확히 올려다볼 때 (Pitch = 90°):
Yaw와 Roll이 같은 동작을 하게 되어 좌우 회전이 이상해짐.

**핵심: 오일러 각은 90° 부근에서 회전이 꼬인다 → 그래서 쿼터니언을 쓴다**

## 오일러 각 vs 쿼터니언

| | 오일러 각 | 쿼터니언 |
|---|---------|----------|
| 직관성 | 이해하기 쉬움 | 수식이 어려움 |
| 짐벌락 | **발생함** | **발생 안 함** |
| 보간 | 부자연스러움 | 부드러운 보간 (Slerp) |
| 용도 | UI 표시, 입력값 | 내부 회전 연산 |

게임 엔진은 내부적으로 **쿼터니언으로 회전을 처리**하고, 사용자에게는 **오일러 각으로 보여주는** 방식.

## 쿼터니언이란?

4개의 숫자 **(x, y, z, w)** 로 회전을 표현.

```
Q = (x, y, z, w)

x, y, z = 회전 축 방향 * sin(θ/2)
w       = cos(θ/2)

θ = 회전 각도
```

**"어떤 축을 중심으로 얼마나 회전했는가"를 4개 숫자로 표현한다**

### 회전 안 한 상태 (단위 쿼터니언)

```
Q = (0, 0, 0, 1)    ← Identity

행렬의 단위 행렬 = 변환 없음
쿼터니언의 (0,0,0,1) = 회전 없음
```

### 예시: Y축으로 90° 회전

```
축 = (0, 1, 0), θ = 90°

x = 0 * sin(45°) = 0
y = 1 * sin(45°) = 0.707
z = 0 * sin(45°) = 0
w = cos(45°) = 0.707

Q = (0, 0.707, 0, 0.707)
```

## 엔진에서의 사용법

### UE

```cpp
// 오일러 → 쿼터니언
FQuat Rotation = FRotator(0, 90, 0).Quaternion();

// 쿼터니언 → 오일러
FRotator Rotator = Rotation.Rotator();

// 회전 합치기
FQuat Combined = RotationA * RotationB;

// 회전 없음
FQuat::Identity
```

### Unity

```csharp
// 오일러 → 쿼터니언
Quaternion rotation = Quaternion.Euler(0, 90, 0);

// 쿼터니언 → 오일러
Vector3 euler = rotation.eulerAngles;

// 회전 합치기
Quaternion combined = rotationA * rotationB;

// 회전 없음
Quaternion.identity
```

### 정리

| 할 일 | UE | Unity |
|-------|-----|-------|
| 오일러 → 쿼터니언 | `FRotator(...).Quaternion()` | `Quaternion.Euler(...)` |
| 쿼터니언 → 오일러 | `.Rotator()` | `.eulerAngles` |
| 회전 합치기 | `Q1 * Q2` | `Q1 * Q2` |
| 회전 없음 | `FQuat::Identity` | `Quaternion.identity` |

실무에서는 **오일러로 입력받고, 쿼터니언으로 변환해서 사용**하는 패턴이 대부분.

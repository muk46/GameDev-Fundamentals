#include <iostream>
#include <cmath>

struct Vector2
{
    float X;
    float Y;
};

// 벡터 뺄셈: From에서 To를 향한 벡터 반환
Vector2 Subtract(Vector2 From, Vector2 To)
{
    return {To.X - From.X, To.Y - From.Y};
}

// 벡터 크기
float Magnitude(Vector2 V)
{
    return sqrt(V.X * V.X + V.Y * V.Y);
}

// TODO: 정규화 함수 구현
Vector2 Normalize(Vector2 V)
{
    // 구현하세요
    float mag = Magnitude(V);
     if (mag == 0) return {0, 0}; // 길이가 0인 벡터는 정규화할 수 없음
	 return { V.X / mag, V.Y / mag };
}

// TODO: 벡터 스칼라 곱 구현
Vector2 Scale(Vector2 V, float S)
{
    // 구현하세요
    return {V.X*S, V.Y*S};
}

// TODO: 벡터 덧셈 구현
Vector2 Add(Vector2 A, Vector2 B)
{
    // 구현하세요
    return { A.X + B.X, A.Y + B.Y };
}

int main()
{
    Vector2 Monster = {3, 1};
    Vector2 Player = {7, 4};
    float Speed = 5.0f;
    float DeltaTime = 1.0f; // 1초

    // TODO: 아래 순서대로 구현하세요
    // 1. 몬스터에서 플레이어를 향한 방향 벡터
    // 2. 정규화
    // 3. 이동량 = 정규화 * 속도 * DeltaTime
    // 4. 새 위치 = 현재 위치 + 이동량
    Vector2 direction = Subtract(Monster, Player);
    Vector2 normalizedDirection = Normalize(direction);
    Vector2 movement = Scale(normalizedDirection, Speed * DeltaTime);
	Vector2 newPosition = Add(Monster, movement);

    std::cout << "[몬스터 이동 시뮬레이션]" << std::endl;
    std::cout << "몬스터 시작 위치: (3, 1)" << std::endl;
    std::cout << "플레이어 위치: (7, 4)" << std::endl;
    std::cout << "속도: 5" << std::endl;

    // 결과 출력하세요
	std::cout << "몬스터 새 위치: (" << newPosition.X << ", " << newPosition.Y << ")" << std::endl;

    return 0;
}

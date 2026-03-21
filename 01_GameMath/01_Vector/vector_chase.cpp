#include <iostream>
#include <cmath>

struct Vector2
{
    float X;
    float Y;
};

Vector2 Subtract(Vector2 From, Vector2 To)
{
    return {To.X - From.X, To.Y - From.Y};
}

float Magnitude(Vector2 V)
{
    return sqrt(V.X * V.X + V.Y * V.Y);
}

Vector2 Normalize(Vector2 V)
{
    float mag = Magnitude(V);
    if (mag < 0.0001f) return {0, 0};
    return {V.X / mag, V.Y / mag};
}

Vector2 Scale(Vector2 V, float S)
{
    return {V.X * S, V.Y * S};
}

Vector2 Add(Vector2 A, Vector2 B)
{
    return {A.X + B.X, A.Y + B.Y};
}

int main()
{
    Vector2 Player = {2, 8};
    float Speed = 10.0f;
    float DeltaTime = 0.5f;

    // 적 3마리
    Vector2 Enemies[] = {{5, 12}, {10, 8}, {6, 5}};
    std::string Names[] = {"A", "B", "C"};
    int EnemyCount = 3;

    // TODO 1: 모든 적까지의 거리를 구하고 출력하세요
    for (int i = 0; i < EnemyCount; i++)
    {
        float distance = Magnitude(Subtract(Player, Enemies[i]));
        std::cout << "적 " << Names[i] << "까지의 거리: " << distance << std::endl;
    }

    // TODO 2: 가장 가까운 적을 찾으세요 (인덱스 저장)
    int closestIndex = 0;
    float minDistance = Magnitude(Subtract(Player, Enemies[0]));

    for (int i = 1; i < EnemyCount; i++)
    {
        float distance = Magnitude(Subtract(Player, Enemies[i]));
        if (distance < minDistance)
        {
            minDistance = distance;
            closestIndex = i;
        }
    }

    // TODO 3: 가장 가까운 적을 향해 이동한 새 위치를 구하세요
    Vector2 direction = Normalize(Subtract(Player, Enemies[closestIndex]));
    Vector2 movement = Scale(direction, Speed * DeltaTime);
    Vector2 newPosition = Add(Player, movement);

    std::cout << "[추적 시스템 - 가장 가까운 적 추적]" << std::endl;
    std::cout << "플레이어 위치: (" << Player.X << ", " << Player.Y << ")" << std::endl;
    std::cout << "속도: " << Speed << " | DeltaTime: " << DeltaTime << std::endl;
    std::cout << "---" << std::endl;
    std::cout << "가장 가까운 적: " << Names[closestIndex] << std::endl;
    std::cout << "새 위치: (" << newPosition.X << ", " << newPosition.Y << ")" << std::endl;

    return 0;
}

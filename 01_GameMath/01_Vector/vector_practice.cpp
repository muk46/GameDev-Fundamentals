#include <iostream>
#include <cmath>
#include <string>

// TODO: 직접 구현해보세요!

struct Vector2
{
    float X;
    float Y;
};

// 벡터 뺄셈: From에서 To를 향한 벡터를 반환
Vector2 Subtract(Vector2 From, Vector2 To)
{
    return {To.X - From.X, To.Y - From.Y};
}

// 벡터 크기(거리) 반환
float Magnitude(Vector2 V)
{
    float distance = sqrt(V.X * V.X + V.Y * V.Y);
    return distance;
}

int main()
{
    Vector2 Tower = {10, 5};
    Vector2 EnemyA = {13, 9};
    Vector2 EnemyB = {7, 2};

    // TODO: 타워에서 각 적까지의 방향 벡터 구하기
    // TODO: 각 거리 구하기
    // TODO: 가장 가까운 적 출력
    float distanceA = Magnitude(Subtract(Tower, EnemyA));
    float distanceB = Magnitude(Subtract(Tower, EnemyB));

    std::cout << "[타워 디펜스 - 가장 가까운 적 찾기]" << std::endl;
    std::cout << "타워 위치: (10, 5)" << std::endl;
    std::cout << "적 A 거리: " << distanceA << std::endl;
    std::cout << "적 B 거리: " << distanceB << std::endl;

    if (distanceA < distanceB)
        std::cout << "가장 가까운 적: Enemy A" << std::endl;
    else
        std::cout << "가장 가까운 적: Enemy B" << std::endl;

    return 0;
}

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

float Dot(Vector2 A, Vector2 B)
{
    return (A.X * B.X + A.Y * B.Y);
}

bool IsInFOV(Vector2 Forward, Vector2 Position, Vector2 Target, float FOVDegrees)
{
    float A = Dot(Forward, Normalize(Subtract(Position, Target)));
    return A >= cos(FOVDegrees * 0.5f * 3.14159265f / 180.0f);
}

int main()
{
    Vector2 Player = {0, 0};
    Vector2 Forward = {1, 0};
    float FOV = 90.0f;

    Vector2 Enemies[] = {{3, 3}, {3, 0}, {-1, 2}, {5, 1}};
    std::string Names[] = {"A", "B", "C", "D"};
    int EnemyCount = 4;

    std::cout << "[시야각 판별 시스템]" << std::endl;
    std::cout << "플레이어 위치: (0, 0) | 전방: (1, 0) | FOV: " << FOV << std::endl;
    std::cout << "---" << std::endl;

    for (int i = 0; i < EnemyCount; i++)
    {
        bool inFOV = IsInFOV(Forward, Player, Enemies[i], FOV);
        std::cout << "적 " << Names[i] << " 위치: (" << Enemies[i].X << ", " << Enemies[i].Y << ") -> ";
        std::cout << (inFOV ? "시야 안" : "시야 밖") << std::endl;
    }

    return 0;
}

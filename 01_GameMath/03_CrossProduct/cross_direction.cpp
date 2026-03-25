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

float Cross(Vector2 A, Vector2 B)
{
    return (A.X * B.Y - A.Y * B.X);
}

void PrintDirection(Vector2 Forward, Vector2 Position, Vector2 Target, std::string Name)
{
    Vector2 dir = Normalize(Subtract(Position, Target));

    float A = Dot(Forward, dir);
    float B = Cross(Forward, dir);

    std::string fb = (A >= 0) ? "앞" : "뒤";
    std::string lr = (B > 0) ? "왼쪽" : "오른쪽";

    std::cout << "적 " << Name << ": " << fb << "-" << lr << std::endl;
}

int main()
{
    Vector2 Player = {0, 0};
    Vector2 Forward = {0, 1};

    Vector2 Enemies[] = {{1, 3}, {-2, 1}, {1, -2}, {-3, -1}};
    std::string Names[] = {"A", "B", "C", "D"};
    int EnemyCount = 4;

    std::cout << "[4방향 판별 시스템]" << std::endl;
    std::cout << "플레이어 위치: (0, 0) | 전방: (0, 1) 위쪽" << std::endl;
    std::cout << "---" << std::endl;

    for (int i = 0; i < EnemyCount; i++)
    {
        PrintDirection(Forward, Player, Enemies[i], Names[i]);
    }

    return 0;
}

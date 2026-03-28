#include <iostream>
#include <cmath>

struct Vector3
{
    float X;
    float Y;
    float W; // 동차 좌표 (항상 1)
};

struct Matrix3x3
{
    float M[3][3];
};

Vector3 Multiply(Matrix3x3 Mat, Vector3 V)
{
    return {
          Mat.M[0][0] * V.X + Mat.M[0][1] * V.Y + Mat.M[0][2] * V.W,
          Mat.M[1][0] * V.X + Mat.M[1][1] * V.Y + Mat.M[1][2] * V.W,
          Mat.M[2][0] * V.X + Mat.M[2][1] * V.Y + Mat.M[2][2] * V.W
    };
}

Matrix3x3 MakeScale(float Sx, float Sy)
{
    Matrix3x3 Mat = {};
    Mat.M[0][0] = Sx;
    Mat.M[1][1] = Sy;
    Mat.M[2][2] = 1;
    return Mat;
}

Matrix3x3 MakeTranslation(float Tx, float Ty)
{
    Matrix3x3 Mat = {};
    Mat.M[0][0] = 1;
    Mat.M[1][1] = 1;
    Mat.M[2][2] = 1;
    Mat.M[0][2] = Tx;
    Mat.M[1][2] = Ty;
    return Mat;
}

Matrix3x3 MakeRotation(float Degrees)
{
    float rad = Degrees * 3.14159265f / 180.0f;
    float c = cos(rad);
    float s = sin(rad);

    Matrix3x3 Mat = {};
    Mat.M[0][0] = c;
    Mat.M[0][1] = -s;
    Mat.M[1][0] = s;
    Mat.M[1][1] = c;
    Mat.M[2][2] = 1;
    return Mat;
}

void PrintVector(std::string label, Vector3 V)
{
    std::cout << label << "(" << V.X << ", " << V.Y << ")" << std::endl;
}

int main()
{
    Vector3 Point = {3, 4, 1};

    std::cout << "[변환 행렬 테스트]" << std::endl;
    PrintVector("원본: ", Point);
    std::cout << "---" << std::endl;

    Vector3 Point1 = Multiply(MakeScale(2, 2), Point);
    PrintVector("스케일 2배: ", Point1);

    Vector3 Point2 = Multiply(MakeTranslation(5, 3), Point);
    PrintVector("이동 (5, 3): ", Point2);

    Vector3 Point3 = Multiply(MakeRotation(90), Point);
    PrintVector("회전 90도: ", Point3);

    return 0;
}

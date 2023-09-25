#pragma once
#include <Matrix4x4.h>
#include <Vector3.h>

// プロトタイプ宣言
Matrix4x4 MakeTranslateMatrix(const Vector3& translate);
Matrix4x4 MakeScaleMatrix(const Vector3& scale);
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);
Matrix4x4 MakeRotateXMatrix(float radian);
Matrix4x4 MakeRotateYMatrix(float radian);
Matrix4x4 MakeRotateZMatrix(float radian);
Matrix4x4 MakeRotateXYZMatrix(Matrix4x4& rotateX, Matrix4x4& rotateY, Matrix4x4& rotateZ);
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);
Vector3 Add(const Vector3& v1, const Vector3& v2);
float Subtract(const float& i1, const float& i2);
Vector3 SubtractV3(const Vector3& v1, const Vector3& v2);
Vector3 Multiply(float scalar, const Vector3& v);
Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);
float Dot(const Vector3& v1, const Vector3& v2);
float Length(const Vector3& v);
Vector3 Normalize(const Vector3& v);
//片側vector3片側変数の減算
Vector3 SubtractOneSideVector3(const Vector3& v3, const int& va);
/// <summary>
/// 逆行列。大量の割り算みたいな感じ
/// </summary>
/// <param name="m1"></param>
/// <returns></returns>
Matrix4x4 Inverse(const Matrix4x4& m1);

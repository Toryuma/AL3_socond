#pragma once
#include <Matrix4x4.h>
#include <Vector3.h>

/********************************************************

オーバーロードは型名変えて呼び名が一緒でも変数のクラスと型で自動で
関数を呼び出してくれる機能。絶対にやっておけ。黒魔術をお前は覚えること
になる。特にVector3とMatrix4x4はやっておけ。
めちゃめちゃ似てて関数の中身違うやつは絶対やっちゃダメ。死ぬ。
黒魔術だから。黒は早い代わりに死ぬ。白は安全だけど死ぬ

********************************************************/

// プロトタイプ宣言
Matrix4x4 MakeTranslateMatrix(const Vector3& translate);
Matrix4x4 MakeScaleMatrix(const Vector3& scale);

Matrix4x4 MakeRotateXMatrix(float radian);
Matrix4x4 MakeRotateYMatrix(float radian);
Matrix4x4 MakeRotateZMatrix(float radian);
Matrix4x4 MakeRotateXYZMatrix(Matrix4x4& rotateX, Matrix4x4& rotateY, Matrix4x4& rotateZ);
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);
//加算(黒魔術)
Vector3 Add(const Vector3& v1, const Vector3& v2);
//減算
float Subtract(const float& i1, const float& i2);
Vector3 Subtract(const Vector3& v1, const Vector3& v2);
//積
Vector3 Multiply(float scalar, const Vector3& v);
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

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

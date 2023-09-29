#pragma once
#include "WorldTransform.h"
#include <Model.h>

class PlayerBullet {
public:
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	bool IsDead() const { return isDead_; }

	void OnCollision();

	Vector3 GetWorldPosition();

	float GetRadius();

	Vector3 GetVelocity();

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;

	uint32_t textureHandle_ = 0u;
	// 速度
	Vector3 velocity_;
	// 寿命
	static const int32_t kLifeTime = 60 * 5;
	// デスタイマー
	int32_t deathTimer_ = kLifeTime;
	// デスフラグ
	bool isDead_ = false;

	const float radius_ = 4.0f;
};
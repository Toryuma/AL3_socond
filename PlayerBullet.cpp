#include "PlayerBullet.h"
#include "MyMath.h"
#include <cassert>

void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	assert(model);
	model_ = model;
	textureHandle_ = TextureManager::Load("black.png");

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	velocity_ = velocity;
}

void PlayerBullet::Update() {
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
	worldTransform_.UpdateMatrix();
}

Vector3 PlayerBullet::GetWorldPosition() {
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

// 半径のゲッター
float PlayerBullet::GetRadius() { return radius_; }

void PlayerBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void PlayerBullet::OnCollision() { isDead_ =true; }
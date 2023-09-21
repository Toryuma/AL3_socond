#pragma once
#include "EnemyBullet.h"
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include "cassert"
#include <list>

class Player;

class Enemy {

public:
	void Initialize(Model* model, uint32_t textureHandle);
	void Update();
	void Draw(ViewProjection& viewProjection);
	// 接近フェーズ
	void moveApproach();
	// 離脱フェーズ
	void moveLeave();
	void Fire();
	~Enemy();
	static const int kFireInterval = 60;
	// 接近フェーズ初期化
	void moveApproachInitialize();

	void SetPlayer(Player* player) { player_ = player; }

	Vector3 GetWorldPosition();

	void OnCollision();

	const std::list<EnemyBullet*>& GetBullets() const { return bullets_; }

	float GetRadius();

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	Vector3 velocity_;
	Player* player_ = nullptr;
	const float radius_ = 4.0f;

	std::list<EnemyBullet*> bullets_;

	enum class Phase {
		Approach,
		Leave,
	};
	Phase phase_ = Phase::Approach; // クラスの中はすべてクラスで返す

	int32_t fireTimer = 0;
};
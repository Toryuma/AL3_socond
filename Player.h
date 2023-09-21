#pragma once
#include "Input.h"
#include "Model.h"
#include "PlayerBullet.h"
#include "Vector3.h"
#include "WorldTransform.h"
#include "cassert"
#include <list>

class Player {

public:
	void Initialize(Model* model, uint32_t textureHandle);

	void Attack();

	Vector3 GetWorldPosition();

	void Update();

	void Draw(ViewProjection& viewProjection);

	~Player();

	void OnCollision();

	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }

	float GetRadius();

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	Input* input_ = nullptr;
	const float radius_ = 4.0f;

	/*PlayerBullet* bullet_;*/
	std::list<PlayerBullet*> bullets_;
};

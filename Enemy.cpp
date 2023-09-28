#include "Enemy.h"
#include "ImGuiManager.h"
#include "MyMath.h"
#include "Player.h"
#include "cassert"
#include <Model.h>
#include <GameScene.h>

void Enemy::Initialize(Model* model, uint32_t textureHandle,Vector3 position) {
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();
	worldTransform_.translation_ =position;
	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};
	velocity_ = {0.0f, 0.0f, 0.0f};
	moveApproachInitialize();
}

Vector3 Enemy::GetWorldPosition() {
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

float Enemy::GetRadius() { return radius_; }


//移籍
void Enemy::Update() {
	//コア消すな
	// worldTransform_.TransferMatrix();
	/*worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);*/




	switch (phase_) {
	case Phase::Approach:
	default:
		// 接近移動
		moveApproach();
		break;

	case Phase::Leave:
		// 離脱移動
		moveLeave();
		break;
	}
	worldTransform_.UpdateMatrix();

	//for (EnemyBullet* bullet : bullets_) {
	//	bullet->Update();
	//}
}

//移籍
void Enemy::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	//for (EnemyBullet* bullet : bullets_) {
	//	bullet->Draw(viewProjection);
	//}
}

//移籍
void Enemy::moveApproach() {
	velocity_ = {0.0f, 0.0f, -0.5f};
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);
	if (worldTransform_.translation_.z < 5.0f) {
		phase_ = Phase::Leave;
	}

	/*Fire();*/
	fireTimer--;
	if (fireTimer <= 0) {
		Fire();
		fireTimer = kFireInterval;
	}
}

void Enemy::moveLeave() {
	velocity_ = {-0.2f, 0.2f, 0.0f};
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);
}

//移籍
void Enemy::Fire() {
	assert(player_);

	const float kBulletSpeed = 3.0f;

	// 弾の速度計算
	// 自キャラのワールド座標
	Vector3 playerPos = player_->GetWorldPosition();
	// 敵キャラのワールド座標
	Vector3 enemyPos = GetWorldPosition();
	// 差分ベクトルを求める
	Vector3 differenceVelocity = Subtract(playerPos, enemyPos);
	// ベクトルの正規化
	differenceVelocity = Normalize(differenceVelocity);
	// ベクトルの長さを早さに合わせる
	Vector3 velocity = {
	    differenceVelocity.x * kBulletSpeed, differenceVelocity.y * kBulletSpeed,
	    differenceVelocity.z * kBulletSpeed};

	/*Vector3 velocity(0, 0, kBulletSpeed);
	velocity = TransformNormal(velocity, worldTransform_.matWorld_);*/

	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);
	gameScene_->AddEnemyBullet(newBullet);
	//bullets_.push_back(newBullet); // gameSceneのAddEnemyBulletを使いnewBulletを登録する
}

//移籍無し
Enemy::~Enemy() {
	/*for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}*/
}

void Enemy::moveApproachInitialize() { fireTimer = kFireInterval; }

void Enemy::OnCollision(){};

void Enemy::SetPlayer(Player* player) { player_ = player; }
void Enemy::SetGameScene(GameScene* gameScene){ gameScene_ = gameScene; }
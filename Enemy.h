#pragma once
#include "EnemyBullet.h"
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include "cassert"
#include <list>

class Player;
class GameScene;
//関数全引っ越し、変数はリストのみ

// エラーは監督の意向によりゲームシーンに移籍とする
// 移籍メンバー以下
/***************************************
void Enemy::Update()
void Enemy::Draw(ViewProjection& viewProjection)
void Enemy::Fire()
Enemy::~Enemy()
const std::list<EnemyBullet*>& GetBullets() const { return bullets_; }
***************************************/

class Enemy {

public:
	void Initialize(Model* model, uint32_t textureHandle, Vector3 position);
	
	// ロッカーは残しているよ
	void Update();
	void Draw(ViewProjection& viewProjection);
	void Fire();
	~Enemy();
	//const std::list<EnemyBullet*>& GetBullets() const { return bullets_; }
	
	
	// 接近フェーズ
	void moveApproach();
	// 離脱フェーズ
	void moveLeave();
	
	
	// 接近フェーズ初期化
	void moveApproachInitialize();

	void SetPlayer(Player* player);
	void SetGameScene(GameScene* gameScene);

	Vector3 GetWorldPosition();

	void OnCollision();

	//const std::list<EnemyBullet*>& GetBullets() const { return bullets_; }

	float GetRadius();

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	Vector3 velocity_;
	Player* player_ = nullptr;
	GameScene* gameScene_ = nullptr; 
	const float radius_ = 4.0f;
	static const int kFireInterval = 60;

	//ゲームシーンに電撃移籍
	/*std::list<EnemyBullet*> bullets_;*/

	enum class Phase {
		Approach,
		Leave,
	};
	Phase phase_ = Phase::Approach; // クラスの中はすべてクラスで返す

	int32_t fireTimer = 0;
};
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
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	/// <param name="textureHandle"></param>
	/// <param name="position"></param>
	void Initialize(Model* model, uint32_t textureHandle, Vector3 position);
	
	// ロッカーは残しているよ
	
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection"></param>
	void Draw(ViewProjection& viewProjection);
	/// <summary>
	/// 発火
	/// </summary>
	void Fire();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Enemy();

	//const std::list<EnemyBullet*>& GetBullets() const { return bullets_; }
	
	
	/// <summary>
	/// 接近フェーズ
	/// </summary>
	void moveApproach();
	/// <summary>
	/// 離脱フェーズ
	/// </summary>
	void moveLeave();
	
	
	/// <summary>
	/// 接近フェーズ初期化
	/// </summary>
	void moveApproachInitialize();

	void SetPlayer(Player* player);
	void SetGameScene(GameScene* gameScene);

	Vector3 GetWorldPosition();
	//当たり判定
	void OnCollision();

	//const std::list<EnemyBullet*>& GetBullets() const { return bullets_; }
	
	/// <summary>
	/// 半径ゲッター
	/// </summary>
	/// <returns></returns>
	float GetRadius();

private:
	//座標
	WorldTransform worldTransform_;
	//モデルデータ
	Model* model_ = nullptr;
	//画像テクスチャ
	uint32_t textureHandle_ = 0u;
	//加速度
	Vector3 velocity_;
	//newで呼ぶときの変数
	Player* player_ = nullptr;
	//
	GameScene* gameScene_ = nullptr; 
	//定数半径
	const float radius_ = 4.0f;
	//定数発火タイマー
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
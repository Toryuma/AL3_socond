#pragma once
#include "Audio.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "Enemy.h"
#include "Input.h"
#include "Model.h"
#include "MyMath.h"
#include "Player.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Skydome.h"
#include "RailCamera.h"
#include <sstream>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	void CheckAllCollisions();

	//弾の登録
	void AddEnemyBullet(EnemyBullet* enemyBuullet);

	void LoadEnemyPopData();

	void UpdateEnemyPopCommands();

	void EnemyPopUP(Vector3 position);

	/*void EnemyInitialize(Model* model, uint32_t textureHandle);
	void EnemyDraw(ViewProjection& viewProjection);
	void EnemyFire();
	void EnemyMoveApproach();
	void EnemyMoveLeave();
	void EnemyMoveApproachInitialize();
	Vector3 EnemyGetWorldPosition();
	void EnemyOnCollision();
	float EnemyGetRadius();
	void EnemySetPlayer(Player* player);*/

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	ViewProjection viewProjection_;
	//テクスチャ
	uint32_t texHandlePlayer_ = 0;
	uint32_t texhandleEnemy_ = 0;
	uint32_t texHandleSkydome_ = 0;
	//モデルデータ
	Model* model_;
	//機体
	Player* player_ = nullptr;
	//Enemy* enemy_ = nullptr;
	//デバッグ用カメラ
	bool isDebugCameraActive_ = false;
	DebugCamera* debugCamera_ = nullptr;
	//天球
	Skydome* skydome_ = nullptr;
	Model* modelSkydome_ = nullptr;
	//レールカメラ
	bool isRailCameraActive_ = false;
	RailCamera* railCamera_=nullptr;

	//nullにする意味はエラーを確認するため
	//孤立した移籍メンバーのリスト君
	std::list<EnemyBullet*> enemyBullets_;
	std::list<Enemy*> enemies_;

	std::stringstream enemyPopCommands;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};

#include "GameScene.h"
#include "AxisIndicator.h"
#include "ImGuiManager.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete player_;
	delete debugCamera_;
	delete enemy_;
	delete skydome_;
	delete modelSkydome_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	texHandlePlayer_ = TextureManager::Load("./Resources/megaMan.jpg");
	texhandleEnemy_ = TextureManager::Load("./Resources/airMan.jpg");
	//texHandleSkydome_ = TextureManager::Load("./Resources/skydome/spaceLine.png");

	model_ = Model::Create();
	viewProjection_.Initialize();
	player_ = new Player();
	player_->Initialize(model_, texHandlePlayer_);
	enemy_ = new Enemy();
	enemy_->Initialize(model_, texhandleEnemy_);
	
	//modelSkydome_ = Model::Create();


	debugCamera_ = new DebugCamera(1000, 440);

	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	enemy_->SetPlayer(player_);

	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	// skydome classの生成と初期化
	skydome_ = new Skydome;
	skydome_ -> Initialize(modelSkydome_,texHandleSkydome_);

	viewProjection_.farZ = 1200;
	viewProjection_.Initialize();

}

void GameScene::Update() {
	player_->Update();
	debugCamera_->Update();

	enemy_->Update();
	skydome_->Update();

	CheckAllCollisions();

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_SPACE)) {
		isDebugCameraActive_ = true;
	}
#endif
	if (isDebugCameraActive_) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;

		viewProjection_.TransferMatrix();
	} else {
		viewProjection_.UpdateMatrix();
	}
}

void GameScene::CheckAllCollisions() {
	Vector3 posA, posB;

	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();

	// 完了
#pragma region // 自キャラと敵弾の当たり判定
	// Aが自キャラBが敵弾
	posA = player_->GetWorldPosition();

	for (EnemyBullet* bullet : enemyBullets) {
		// 敵弾の座標
		posB = bullet->GetWorldPosition();

		float r1 = player_->GetRadius();
		float r2 = bullet->GetRadius();
		// 球と球の交差判定
		if ((posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y) +
		        (posB.z - posA.z) * (posB.z - posA.z) <=
		    ((r1 + r2) * (r1 + r2))) {

			player_->OnCollision();
			bullet->OnCollision();
		}
	}

#pragma endregion

#pragma region // 敵キャラと自弾の当たり判定
	// Bが自弾、Aが敵キャラ
	posA = enemy_->GetWorldPosition();

	for (PlayerBullet* bullet : playerBullets) {
		// 敵弾の座標
		posB = bullet->GetWorldPosition();

		float r1 = enemy_->GetRadius();
		float r2 = bullet->GetRadius();
		// 球と球の交差判定
		if ((posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y) +
		        (posB.z - posA.z) * (posB.z - posA.z) <=
		    ((r1 + r2) * (r1 + r2))) {

			enemy_->OnCollision();
			bullet->OnCollision();
		}
	}
#pragma endregion

#pragma region // 自弾と敵弾の当たり判定

	//真ん中はfor文のiのようなもの,しっかりと名前を付けること。被るとエラーが起きる。
	for (PlayerBullet* playerBullet : playerBullets) {
		for (EnemyBullet* enemyBullet : enemyBullets) {

			//自弾の座標
			posA = playerBullet->GetWorldPosition();
			// 敵弾の座標
			posB = enemyBullet->GetWorldPosition();

			float r1 = playerBullet->GetRadius();
			float r2 = enemyBullet->GetRadius();
			// 球と球の交差判定
			if ((posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y) +
			        (posB.z - posA.z) * (posB.z - posA.z) <=
			    ((r1 + r2) * (r1 + r2))) {

				enemyBullet->OnCollision();
				playerBullet->OnCollision();
			}
		}
	}

#pragma endregion
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	skydome_->Draw(viewProjection_);
	player_->Draw(viewProjection_);

	if (enemy_ != nullptr) {
		enemy_->Draw(viewProjection_);
	}
	/// </summary>



	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

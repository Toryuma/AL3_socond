#include "GameScene.h"
#include "AxisIndicator.h"
#include "ImGuiManager.h"
#include "TextureManager.h"
#include <cassert>
#include <fstream>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete player_;
	delete debugCamera_;
	//メモリから削除
	for (Enemy* enemy : enemies_) {
		delete enemy;
	};
	delete skydome_;
	delete modelSkydome_;
	delete railCamera_;
	for (EnemyBullet* bullet : enemyBullets_) {
		delete bullet;
	};
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// 画像読み込み
	texHandlePlayer_ = TextureManager::Load("./Resources/megaMan.jpg");
	texhandleEnemy_ = TextureManager::Load("./Resources/airMan.jpg");
	// texHandleSkydome_ = TextureManager::Load("./Resources/skydome/spaceLine.png");

	// 初期化泳げタイ焼きくん
	model_ = Model::Create();
	viewProjection_.Initialize();

	Vector3 playerPosition(0, 0, 10);
	player_ = new Player();
	player_->Initialize(model_, texHandlePlayer_, playerPosition);
	/*enemy_ = new Enemy();
	enemy_->Initialize(model_, texhandleEnemy_, {});*/

	// modelSkydome_ = Model::Create();

	debugCamera_ = new DebugCamera(1000, 440);

	// 左手フレミング表示
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	// enemy_->SetPlayer(player_);
	// enemy_->SetGameScene(this);
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	// skydome classの生成と初期化
	skydome_ = new Skydome;
	skydome_->Initialize(modelSkydome_, texHandleSkydome_);

	// 天球とのカメラの距離合わせ
	viewProjection_.farZ = 1100;
	viewProjection_.Initialize();

	railCamera_ = new RailCamera();
	railCamera_->Initialize(model_);

	player_->Initialize(model_, texHandlePlayer_, playerPosition);
	// 親子関係を結ぶ
	railCamera_->GetWorldTransform();

	player_->SetParent(&railCamera_->GetWorldTransform());

	LoadEnemyPopData();
}

void GameScene::Update() {
	player_->Update();

	// enemy_->Update();

	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Update();
	}
	UpdateEnemyPopCommands();
	for (Enemy* enemy : enemies_) {
		enemy->Update();
	}

	enemyBullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	skydome_->Update();

	CheckAllCollisions();

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_SPACE)) {
		isDebugCameraActive_ = true;
	}
#endif
	if (isDebugCameraActive_ == true) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;

		viewProjection_.TransferMatrix();
		// デバッグカメラを使っていない場合はレールカメラ
	} else {
		railCamera_->Update();
		viewProjection_.matView = railCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;

		viewProjection_.TransferMatrix();

		// デバッグ以外でUpdateMatrixを呼ばないように(matrixがゴミになりゅ)
		/*viewProjection_.UpdateMatrix();*/
	}
}

void GameScene::CheckAllCollisions() {
	Vector3 posA, posB;

	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	// const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();

	// 完了
#pragma region // 自キャラと敵弾の当たり判定
	// Aが自キャラBが敵弾
	posA = player_->GetWorldPosition();

	for (EnemyBullet* bullet : enemyBullets_) {
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
	for (Enemy* enemy : enemies_) {
		//enemy->Update();

		// Bが自弾、Aが敵キャラ
		posA = enemy->GetWorldPosition();

		for (PlayerBullet* bullet : playerBullets) {
			// 敵弾の座標
			posB = bullet->GetWorldPosition();

			float r1 = enemy->GetRadius();
			float r2 = bullet->GetRadius();
			// 球と球の交差判定
			if ((posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y) +
			        (posB.z - posA.z) * (posB.z - posA.z) <=
			    ((r1 + r2) * (r1 + r2))) {

				enemy->OnCollision();
				bullet->OnCollision();
			}
		}
	}
#pragma endregion

#pragma region // 自弾と敵弾の当たり判定

	// 真ん中はfor文のiのようなもの,しっかりと名前を付けること。被るとエラーが起きる。
	for (PlayerBullet* playerBullet : playerBullets) {
		for (EnemyBullet* enemyBullet : enemyBullets_) {

			// 自弾の座標
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

void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) {
	// リスト登録
	enemyBullets_.push_back(enemyBullet);
}

void GameScene::LoadEnemyPopData() {
	std::ifstream file;
	file.open("./Resources/enemyPop.csv");

	assert(file.is_open());

	enemyPopCommands << file.rdbuf();

	file.close();
}

void GameScene::UpdateEnemyPopCommands() {
	std::string line;

	while (getline(enemyPopCommands, line)) {
		std::istringstream line_stream(line);

		std::string word;

		getline(line_stream, word, ',');

		if (word.find("//") == 0) {
			continue;
		}

		if (word.find("POP") == 0) {

			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			EnemyPopUP({x, y, z});
		}
	}
}

void GameScene::EnemyPopUP(Vector3 position) {
	Enemy* enemy = new Enemy();
	// 初期化
	enemy->Initialize(model_, texhandleEnemy_, position);
	enemy->SetPlayer(player_);
	enemy->SetGameScene(this);

	enemies_.push_back(enemy);
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

	for (Enemy* enemy : enemies_) {
		enemy->Draw(viewProjection_);
	}

	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Draw(viewProjection_);
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
/************************************
 * 　　　　　　　　移籍隔離所
 ************************************/

//
// void GameScene::EnemyDraw(ViewProjection& viewProjection) {
//	model_->Draw(worldTransform_, viewProjection, textureHandle_);
//
//	for (EnemyBullet* bullet : bullets_) {
//		bullet->Draw(viewProjection);
//	}
//}
//
// void GameScene::EnemyFire() {
//	assert(player_);
//
//	const float kBulletSpeed = 3.0f;
//
//	// 弾の速度計算
//	// 自キャラのワールド座標
//	Vector3 playerPos = player_->GetWorldPosition();
//	// 敵キャラのワールド座標
//	Vector3 enemyPos = GetWorldPosition();
//	// 差分ベクトルを求める
//	Vector3 differenceVelocity = Subtract(playerPos, enemyPos);
//	// ベクトルの正規化
//	differenceVelocity = Normalize(differenceVelocity);
//	// ベクトルの長さを早さに合わせる
//	Vector3 velocity = {
//	    differenceVelocity.x * kBulletSpeed, differenceVelocity.y * kBulletSpeed,
//	    differenceVelocity.z * kBulletSpeed};
//
//	/*Vector3 velocity(0, 0, kBulletSpeed);
//	velocity = TransformNormal(velocity, worldTransform_.matWorld_);*/
//
//	EnemyBullet* newBullet = new EnemyBullet();
//	newBullet->Initialize(model_, worldTransform_.translation_, velocity);
//	bullets_.push_back(newBullet);
//}
//
// void GameScene::EnemyMoveApproach() {
//	velocity_ = {0.0f, 0.0f, -0.5f};
//	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);
//	if (worldTransform_.translation_.z < 5.0f) {
//		phase_ = Phase::Leave;
//	}
//
//	/*Fire();*/
//	fireTimer--;
//	if (fireTimer <= 0) {
//		Fire();
//		fireTimer = kFireInterval;
//	}
//}
//
// void GameScene::EnemyInitialize(Model* model, uint32_t textureHandle) {
//	assert(model);
//
//	model_ = model;
//	textureHandle_ = textureHandle;
//
//	worldTransform_.Initialize();
//	worldTransform_.translation_ = {0.0f, 2.0f, 200.0f};
//	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};
//	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};
//	velocity_ = {0.0f, 0.0f, 0.0f};
//	moveApproachInitialize();
//}
//
// Vector3 GameScene::EnemyGetWorldPosition() {
//	Vector3 worldPos;
//
//	worldPos.x = worldTransform_.matWorld_.m[3][0];
//	worldPos.y = worldTransform_.matWorld_.m[3][1];
//	worldPos.z = worldTransform_.matWorld_.m[3][2];
//
//	return worldPos;
//}
//
// float GameScene::EnemyGetRadius() { return radius_; }
//
// void GameScene::EnemyMoveLeave() {
//	velocity_ = {-0.2f, 0.2f, 0.0f};
//	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);
//}
//
// void GameScene::EnemyMoveApproachInitialize() { Enemy::fireTimer = kFireInterval; }
//
// void GameScene::EnemyOnCollision(){};
//
// void GameScene::EnemySetPlayer(Player* player) { player_ = player; };
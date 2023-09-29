#include "Player.h"
#include "ImGuiManager.h"
#include "cassert"
#include <MyMath.h>

void Player::Initialize(Model* model, uint32_t textureHandle, Vector3 playerPosition) {

	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();
	input_ = Input::GetInstance();
	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.translation_ = playerPosition;
	worldTransform3DReticle_.Initialize();
}

void Player::Attack() {
	// 発射トリガー
	if (input_->PushKey(DIK_SPACE)) {

		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		//登録前に角度計算
		velocity = Subtract(GetWorldPosition3DReticle(), GetWorldPosition());
		velocity = Multiply(kBulletSpeed,Normalize(velocity));

		//弾の登録
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, worldTransform_.translation_, velocity);
		bullets_.push_back(newBullet);
		GetWorldPosition(); // プレイヤーのワールド座標	
		GetWorldPosition3DReticle();
		
		/*
			Vector3 worldPos;

			worldPos.x = worldTransform_.matWorld_.m[3][0];
			worldPos.y = worldTransform_.matWorld_.m[3][1];
			worldPos.z = worldTransform_.matWorld_.m[3][2];

			Vector3 world3DPos;

			world3DPos.x = worldTransform3DReticle_.matWorld_.m[3][0];
			world3DPos.y = worldTransform3DReticle_.matWorld_.m[3][1];
			world3DPos.z = worldTransform3DReticle_.matWorld_.m[3][2];

			Subtract(world3DPos, worldPos);
		*/
	}

	
}

Vector3 Player::GetWorldPosition() {
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}
Vector3 Player::GetWorldPosition3DReticle() {
	Vector3 world3DPos;

	world3DPos.x = worldTransform3DReticle_.matWorld_.m[3][0];
	world3DPos.y = worldTransform3DReticle_.matWorld_.m[3][1];
	world3DPos.z = worldTransform3DReticle_.matWorld_.m[3][2];

	return world3DPos;
}

//半径のゲッター
float Player::GetRadius() { 
	return radius_; }

void Player::Update() {
	//worldTransform_.TransferMatrix();

	//// スケーリング行列宣言
	//Matrix4x4 matScale = {0};
	//matScale.m[0][0] = worldTransform_.scale_.x;
	//matScale.m[1][1] = worldTransform_.scale_.y;
	//matScale.m[2][2] = worldTransform_.scale_.z;
	//matScale.m[3][3] = 1;

	//Matrix4x4 matRotX = {0};
	//matRotX.m[0][0] = 1;
	//matRotX.m[1][1] = cosf(worldTransform_.rotation_.x);
	//matRotX.m[2][1] = -sinf(worldTransform_.rotation_.x);
	//matRotX.m[1][2] = sinf(worldTransform_.rotation_.x);
	//matRotX.m[2][2] = cosf(worldTransform_.rotation_.x);
	//matRotX.m[3][3] = 1;

	//Matrix4x4 matRotY = {0};
	//matRotY.m[0][0] = cosf(worldTransform_.rotation_.y);
	//matRotY.m[1][1] = 1;
	//matRotY.m[0][2] = -sinf(worldTransform_.rotation_.y);
	//matRotY.m[2][0] = sinf(worldTransform_.rotation_.y);
	//matRotY.m[2][2] = cosf(worldTransform_.rotation_.y);
	//matRotY.m[3][3] = 1;

	//Matrix4x4 matRotZ = {0};
	//matRotZ.m[0][0] = cosf(worldTransform_.rotation_.z);
	//matRotZ.m[1][0] = sinf(worldTransform_.rotation_.z);
	//matRotZ.m[0][1] = -sinf(worldTransform_.rotation_.z);
	//matRotZ.m[1][1] = cosf(worldTransform_.rotation_.z);
	//matRotZ.m[2][2] = 1;
	//matRotZ.m[3][3] = 1;

	//Matrix4x4 matRot = MakeRotateXYZMatrix(matRotX, matRotY, matRotZ);
	//Matrix4x4 matTrans = MakeTranslateMatrix(worldTransform_.translation_);

	//worldTransform_.matWorld_ = MakeAffineMatrix(
	//    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	worldTransform_.UpdateMatrix();

	// デスフラグの立った弾を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	// 旋回処理
	const float kRotSpeed = 0.02f;
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y = worldTransform_.rotation_.y - kRotSpeed;
	} else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y = worldTransform_.rotation_.y + kRotSpeed;
	}

	// 移動処理
	Vector3 move = {0, 0, 0};
	const float kCharacterSpeed = 0.2f;
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}
	if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	}
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);

	// 移動限界座標
	const float kMoveLimitX = 10;
	const float kMoveLimitY = 10;

	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	// 攻撃処理
	Attack();

	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}

	////bullet_はif文だとbullet_ != nullptrと同じ
	// if (bullets_) {
	//	bullets_->Update();
	// }

	ImGui::Begin("Debug");
	float playerPos[] = {
	    worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z};
	ImGui::SliderFloat3("PlayerPos", playerPos, 25, -25);

	worldTransform_.translation_.x = playerPos[0];
	worldTransform_.translation_.y = playerPos[1];
	worldTransform_.translation_.z = playerPos[2];
	ImGui::End();

	{ 
		const float kDistancePlayerTo3DReticle = 50.0f;
		Vector3 offset = {0, 0, 1.0f};

		offset = TransformNormal(offset, worldTransform_.matWorld_);
		offset = Multiply(kDistancePlayerTo3DReticle, Normalize(offset));
		worldTransform3DReticle_.translation_ = Add(worldTransform_.translation_,offset);
		
		worldTransform3DReticle_.UpdateMatrix();

	}
}

void Player::SetParent(const WorldTransform* parent) { worldTransform_.parent_ = parent; }



void Player::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	// if (bullets_) {
	//	bullets_->Draw(viewProjection);
	// }

	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}

	model_->Draw(worldTransform3DReticle_, viewProjection);
}

Player::~Player() {
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
}

void Player::OnCollision(){};

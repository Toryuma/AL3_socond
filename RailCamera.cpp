#include "RailCamera.h"
#include "ImGuiManager.h"
#include "cassert"
#include <MyMath.h>
// 宣言
void RailCamera::Initialize(Model* model) {
	assert(model);

	model_ = model;

	worldTransform_.Initialize();
	input_ = Input::GetInstance();
	worldTransform_.scale_ = {1.0f, 1.0f, -1.0f};
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};

	// 初期改造
	worldTransform_;
	worldTransform_.rotation_;

	viewProjection_.farZ = 50.0;
	viewProjection_.Initialize();
}
// 更新
void RailCamera::Update() {

	// 回転
	const float kRotSpeed = 0.1f;
	if (input_->PushKey(DIK_G)) {
		worldTransform_.rotation_.z = worldTransform_.rotation_.z - kRotSpeed;
	} else if (input_->PushKey(DIK_H)) {
		worldTransform_.rotation_.z = worldTransform_.rotation_.z + kRotSpeed;
	}

	// 移動

	// 操作方法変えること
	// 追従操作はその分自機も動くため描画の位置が変わらず分からない
	// すべて外部かこの中で作られていて、独立している。
	Vector3 move = {0, 0, 0};
	const float kCharacterSpeed = 0.2f;
	if (input_->PushKey(DIK_J)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_L)) {
		move.x += kCharacterSpeed;
	}
	if (input_->PushKey(DIK_I)) {
		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_K)) {
		move.y -= kCharacterSpeed;
	}
	if (input_->PushKey(DIK_N)) {
		move.z += kCharacterSpeed;
	} else if (input_->PushKey(DIK_M)) {
		move.z -= kCharacterSpeed;
	}
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);
	// ここまでが変数
	worldTransform_.UpdateMatrix();

	viewProjection_.matView = Inverse(worldTransform_.matWorld_);

	ImGui::Begin("Debug");
	float railCameraPos[] = {
	    worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z};
	ImGui::SliderFloat3("railCameraPos", railCameraPos, 25, -25);

	worldTransform_.translation_.x = railCameraPos[0];
	worldTransform_.translation_.y = railCameraPos[1];
	worldTransform_.translation_.z = railCameraPos[2];
	ImGui::End();
}

const WorldTransform& RailCamera::GetWorldTransform() { return worldTransform_; }


// デバッグ用にDrawを作ってもいい
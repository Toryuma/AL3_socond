#include "RailCamera.h"
#include "ImGuiManager.h"
#include "cassert"
#include <MyMath.h>
//宣言
void RailCamera::Initialize(Model* model,Vector3* v3) {
	assert(model);

	model_ = model;

	worldTransform_.Initialize();
	input_ = Input::GetInstance();
	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};

	//初期改造
	worldTransform_;
	worldTransform_.rotation_;

	viewProjection_.Initialize();
}
//更新
void RailCamera::Update() { 
	
	//回転
	const float kRotSpeed = 0.00f;
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y = worldTransform_.rotation_.y - kRotSpeed;
	} else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y = worldTransform_.rotation_.y + kRotSpeed;
	}

	//移動
	worldTransform_.UpdateMatrix();
	
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);

	ImGui::Begin("Camera");
	ImGui::
	ImGui::End;
}

// デバッグ用にDrawを作ってもいい
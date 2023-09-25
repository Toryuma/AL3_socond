#include "Skydome.h"
#include "ImGuiManager.h"
#include "cassert"
#include <Model.h>
#include <MyMath.h>

void Skydome::Initialize(Model* model, uint32_t textureHandle) {

	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();
	input_ = Input::GetInstance();
	worldTransform_.scale_ = {40.0f, 40.0f, 40.0f};
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};

	// 行列の更新(アフィン変換)、GPU転送(シェーダーに)
	worldTransform_.UpdateMatrix();

	// . (ドット演算子)ポインタ変数ではないクラスまたは構造体、enumの変数に使う
	// ->(アロー演算子)ポインタ変数であるクラスまたは構造体の変数に使う
};

void Skydome::Update() { worldTransform_.UpdateMatrix(); };

void Skydome::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection);
}
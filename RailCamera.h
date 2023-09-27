#pragma once
#include "WorldTransform.h"
#include "cassert"
#include "ViewProjection.h"
#include "Input.h"
#include "Model.h"
#include "Vector3.h"

class RailCamera {
public:
	void Initialize(Model* model);

	void Update();

	const ViewProjection& GetViewProjection() { return viewProjection_; }

	// Shift+ F12,名前選択右クリック、すべての参照の検索でこの関数がどこに使われているかが出る
	const WorldTransform& GetWorldTransform();

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	ViewProjection viewProjection_;
	Input* input_ = nullptr;
	RailCamera* railCamera_ = nullptr;
};

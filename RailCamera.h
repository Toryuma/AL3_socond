#pragma once
#include "WorldTransform.h"
#include "cassert"
#include "ViewProjection.h"
#include "Input.h"
#include "Model.h"
#include "Vector3.h"

class RailCamera {
public:
	void Initialize(Model* model, Vector3* v3);

	void Update();

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	ViewProjection viewProjection_;
	Input* input_ = nullptr;
	RailCamera* railCamera_ = nullptr;
};

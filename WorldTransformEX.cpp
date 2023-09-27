#include <MyMath.h>
#include <WorldTransform.h>
#include <assert.h>

void WorldTransform::UpdateMatrix() {

	matWorld_ = MakeAffineMatrix(scale_, rotation_, translation_);

	// ごっみキーボード編っ客安定だあっしねっかす
	//  親子関係の反映
	// アロー演算子のため記号の*は使わん
	/*if (parent_) {
	* matWorld_ = matWorld_ * parrent_->matWorld_;
	    matWorld_ *= parent_->matWorld_;
	}*/

	//ifつけないとnullが通ってしまう
	/*Matrix4x4 worldTransform_ = parent_->matWorld_;*/

	//同じ能力持ってるやつじゃないと関数は動かん

	//nullか確認する条件式
	if (parent_) {
		matWorld_ = Multiply(matWorld_, parent_->matWorld_);
	}

	TransferMatrix();
}
#pragma once
#include "Input.h"
#include "Model.h"
#include "PlayerBullet.h"
#include "Vector3.h"
#include "WorldTransform.h"
#include "cassert"
#include <list>

class Player {

public:
	/// <summary>
	/// 初期化。お前これ消すとか正気か?
	/// </summary>
	/// <param name="model"></param>
	/// <param name="textureHandle"></param>
	void Initialize(Model* model, uint32_t textureHandle,Vector3 playerPosition);
	/// <summary>
	/// 攻撃。受け身に回るな、ぶちのめせ！！
	/// </summary>
	void Attack();
	/// <summary>
	/// 世界の位置が知れる。身の程をわきまえよう。
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition();
	/// <summary>
	/// 更新。彼女いない歴=年齢は常に更新されています。彼女募集。
	/// </summary>
	
	

	void Update();
	/// <summary>
	/// 描画。絵描けると女性との話題に意外と困らない。最重要案件
	/// </summary>
	/// <param name="viewProjection"></param>
	void Draw(ViewProjection& viewProjection);

	~Player();

	/// <summary>
	/// 接触判定。JKと接触すると捕まる
	/// </summary>
	void OnCollision();

	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }

	//関数用のコメントアウト.基本ヘッダーに書く。スラッシュ三回で展開
	/// <summary>
	/// 半径のゲッター。俺が小さい時見ていたのはヤッターマン
	/// </summary>
	/// <returns></returns>
	float GetRadius();

	void SetParent(const WorldTransform* parent);

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	Input* input_ = nullptr;
	const float radius_ = 4.0f;

	/*PlayerBullet* bullet_;*/
	std::list<PlayerBullet*> bullets_;
};

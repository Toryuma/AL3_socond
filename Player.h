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
	/// �������B���O��������Ƃ����C��?
	/// </summary>
	/// <param name="model"></param>
	/// <param name="textureHandle"></param>
	void Initialize(Model* model, uint32_t textureHandle);
	/// <summary>
	/// �U���B�󂯐g�ɉ��ȁA�Ԃ��̂߂��I�I
	/// </summary>
	void Attack();
	/// <summary>
	/// ���E�̈ʒu���m���B�g�̒����킫�܂��悤�B
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition();
	/// <summary>
	/// �X�V�B�ޏ����Ȃ���=�N��͏�ɍX�V����Ă��܂��B�ޏ���W�B
	/// </summary>
	void Update();
	/// <summary>
	/// �`��B�G�`����Ə����Ƃ̘b��ɈӊO�ƍ���Ȃ��B�ŏd�v�Č�
	/// </summary>
	/// <param name="viewProjection"></param>
	void Draw(ViewProjection& viewProjection);

	~Player();

	/// <summary>
	/// �ڐG����BJK�ƐڐG����ƕ߂܂�
	/// </summary>
	void OnCollision();

	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }

	//�֐��p�̃R�����g�A�E�g.��{�w�b�_�[�ɏ����B�X���b�V���O��œW�J
	/// <summary>
	/// ���a�̃Q�b�^�[�B���������������Ă����̂̓��b�^�[�}��
	/// </summary>
	/// <returns></returns>
	float GetRadius();

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	Input* input_ = nullptr;
	const float radius_ = 4.0f;

	/*PlayerBullet* bullet_;*/
	std::list<PlayerBullet*> bullets_;
};

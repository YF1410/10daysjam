#pragma once
#include "Object3d.h"
#include "FbxObject3d.h"
#include "CollisionPrimitive.h"
#include "Player.h"
#include "Collision.h"
#include <random>
#include"ParticleManager.h"
#include "SphereCollider.h"

class Player;

class BaseEnemy : public FbxObject3d
{
protected: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	static std::unique_ptr<BaseEnemy> Create(Player* player,Camera*camera);
public:
	//コンストラクタ
	BaseEnemy();
	//デストラクタ
	~BaseEnemy();
	//初期化
	void Initialize(Player*player,Camera*camera);
	//更新
	virtual void Update();

	void CollisionUpdate();

	virtual void Draw(ID3D12GraphicsCommandList* cmdList);

	void randPosX(float posDist);
	void randPosZ(float posDist);
	void randPos();

	//ゲッター
	bool GetIsActive() { return isActive; }
	Sphere GetCollision() { return collision; }
	bool GetHaveElement() { return haveElement; }
	//セッター
	void SetIsDamage(bool isDamage) { this->isDamage = isDamage; }

	void SetIsFirstDamage(bool isFirstDamage) { this->isFirstDamage = isFirstDamage; }
	//ダメージ処理
	virtual void Damage();

	void CheckCollisionToPlayer();

	virtual void Move();
protected:
	Player* player;
	Camera* camera;
	float HP = 3;
	XMFLOAT3 savePos;
	XMFLOAT4 defaultColor = { 0,1,1,1 };
	bool isDamage = false;
	bool isFirstDamage = false;
	bool isActive = true;
	bool haveElement = false;

	bool isDecisionPosX = false;
	bool isDecisionPosZ = false;

	int damageShakeCount = 0;
	float shakeObjectPos[3];
	float moveX = 0;
	float moveZ = 0;
	float moveAmount = 0.2f;
	XMFLOAT3 defaultPos = {0,-5.0f,20.0f};
	Sphere collision;
	SphereCollider sphereCollision;
	std::unique_ptr<Model> colliderVisualizationModel;
	std::unique_ptr<Object3d> colliderVisualizationObject;
protected:
	static std::unique_ptr<FbxModel> enemyModel;
};


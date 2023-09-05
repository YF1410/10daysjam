#pragma once

#include "SafeDelete.h"
#include "DirectXCommon.h"
#include <DirectXMath.h>
#include "Input.h"
#include "Sprite.h"
#include "ParticleManager.h"
#include "DebugText.h"
#include "Audio.h"
#include "DebugCamera.h"
#include "Object3d.h"
#include "FbxObject3d.h"
#include "CollisionPrimitive.h"

#include "SceneManager.h"
#include "SceneInterface.h"
#include "BaseCollider.h"
#include <vector>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene : public SceneInterface {
protected: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

protected: // 静的メンバ変数
	static const int debugTextTexNumber = 0;
public: // メンバ関数

	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	void Finalize() override;

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

protected: // メンバ変数
	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;
	Audio* audio = nullptr;
	DebugText* debugText = nullptr;

	std::unique_ptr<Sprite> fadeSprite;

	std::unique_ptr<Sprite> blockSprite[3];//ブロックの種類(0:〇、1:△、2:♢)
	std::unique_ptr<Sprite> cursor;

	XMFLOAT4 fadeColor = { 1.0f,1.0f,1.0f,1.0f };
	bool isFadeOut = false;
	bool isFadeIn = true;
	bool isGameOver = false;
	bool isGameClear = false;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	std::unique_ptr <Camera> cameraObject;
	ParticleManager* particleMan;

	std::unique_ptr<LightGroup> lightGroup;

	float cameraEye[3] = { 0.0f,25.0f,-50.0f };
	XMFLOAT3 cameraTarget = { 0.0f,0.0f,0.0f };
	Sphere cameraCollider;
	Sphere skydomeCollider;
	float posX;
	float posZ;

	XMFLOAT3 circleShadowDir = {0,-1,0};
	XMFLOAT3 circleShadowAtten = { 0.5f,0.6f,0.0f };
	XMFLOAT2 circleShadowFactorAngle = { 0.0f,0.5f };
};
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

class ResultScene : public SceneInterface
{
protected: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	void Initialize()override;
	void Finalize()override;
	void Update()override;
	void Draw()override;

protected: // メンバ変数
	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;
	Audio* audio = nullptr;

	SceneManager* scene_;


	std::unique_ptr<Sprite> fadeSprite;
	std::unique_ptr<Sprite> cursor;
	XMFLOAT2 saveCursorPos = { 0.0f,0.0f };
	std::unique_ptr<Sprite> backGround;//ゲーム背景

	//スコア関連
	std::unique_ptr<Sprite> scoreSprite;//SCOREロゴ
	std::unique_ptr<Sprite> colonScore;//コロン(:)
	std::unique_ptr<Sprite> hunThouScore[10];//[0]00000
	std::unique_ptr<Sprite> tenThouScore[10];//0[0]0000
	std::unique_ptr<Sprite> thouScore[10];//00[0]000
	std::unique_ptr<Sprite> hunScore[10];//000[0]00
	std::unique_ptr<Sprite> tenScore[10];//0000[0]0
	std::unique_ptr<Sprite> oneScore[10];//00000[0]

	//リザルト画面用
	std::unique_ptr<Sprite> retrySprite;//RETRYロゴ
	std::unique_ptr<Sprite> titleSprite;//TITLEロゴ
	std::unique_ptr<Sprite> resultSprite;//RESULTロゴ
	std::unique_ptr<Sprite> rankSprite;//RANKロゴ
	std::unique_ptr<Sprite> aSprite;//aロゴ
	std::unique_ptr<Sprite> bSprite;//bロゴ
	std::unique_ptr<Sprite> cSprite;//cロゴ

	XMFLOAT4 fadeColor = { 1.0f,1.0f,1.0f,1.0f };
	bool isFadeOut = false;
	bool isFadeIn = true;
	bool isGameOver = false;
	bool isGameClear = false;
	bool isTitle = false;
	bool isRetry = false;

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

	XMFLOAT3 circleShadowDir = { 0,-1,0 };
	XMFLOAT3 circleShadowAtten = { 0.5f,0.6f,0.0f };
	XMFLOAT2 circleShadowFactorAngle = { 0.0f,0.5f };

	//スコア関連
	int preScore = 0;//加算前のスコア
	int score = 0;//スコア
	int drawScore[6];//スコア表示用(0:十万 1:万 2:千 3:百 4:十 5:一)
	float rankPattern = 0.0f;//RANK表示(0:A 1:B 2:C)
};


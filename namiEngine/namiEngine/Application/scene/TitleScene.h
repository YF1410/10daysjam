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

class TitleScene : public SceneInterface
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

	//ブロックパターン取得処理
	void GetBreakPattern(float block);

	//スコア獲得処理
	void GetScore();

	//スコア加算処理
	void ScoreCharge();

protected: // メンバ変数
	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;
	Audio* audio = nullptr;

	SceneManager* scene_;

	std::unique_ptr<Sprite> cursor;
	XMFLOAT2 saveCursorPos = { 0.0f,0.0f };
	std::unique_ptr<Sprite> tile[4][4];
	float tileSize = 128.0f;
	bool xFlag = false;
	bool yFlag = false;
	std::unique_ptr<Sprite> backGround;//ゲーム背景

	std::unique_ptr<Sprite> circle[8][4];//画面左のブロック(〇)
	std::unique_ptr<Sprite> triangle[8][4];//画面左のブロック(△)
	std::unique_ptr<Sprite> square[8][4];//画面左のブロック(♢)

	//タイトル画面関連
	std::unique_ptr<Sprite> pointCircle;//POINT〇
	std::unique_ptr<Sprite> pointTriangle;//POINT△
	std::unique_ptr<Sprite> pointSquare;//POINT♢
	std::unique_ptr<Sprite> clickDrag;//CLICK&DRAGロゴ
	std::unique_ptr<Sprite> startSprite;//STARTロゴ

	//スコア関連
	std::unique_ptr<Sprite> scoreSprite;//SCOREロゴ
	std::unique_ptr<Sprite> colonScore;//コロン(:)
	std::unique_ptr<Sprite> hunThouScore[10];//[0]00000
	std::unique_ptr<Sprite> tenThouScore[10];//0[0]0000
	std::unique_ptr<Sprite> thouScore[10];//00[0]000
	std::unique_ptr<Sprite> hunScore[10];//000[0]00
	std::unique_ptr<Sprite> tenScore[10];//0000[0]0
	std::unique_ptr<Sprite> oneScore[10];//00000[0]

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

	XMFLOAT3 circleShadowDir = { 0,-1,0 };
	XMFLOAT3 circleShadowAtten = { 0.5f,0.6f,0.0f };
	XMFLOAT2 circleShadowFactorAngle = { 0.0f,0.5f };

	//スコア関連
	int preScore = 0;//加算前のスコア
	int score = 0;//スコア
	int drawScore[6];//スコア表示用(0:十万 1:万 2:千 3:百 4:十 5:一)
	float breakPattern[10];//消すブロックの種類

	//ブロック表示関連
	float tilePattern[8][4];//画面左のブロック配置(0:なし 1:〇 2:△ 3:♢)
	bool circleArive[8][4];//画面左の〇の表示(0:非表示 1:表示)
	bool triangleArive[8][4];//画面左の△の表示(0:非表示 1:表示)
	bool squareArive[8][4];//画面左の♢の表示(0:非表示 1:表示)
	float pointPattern = 0.0f;//Pointボードの見た目(0:〇 1:△ 2:♢)
	float pointChange = 0.0f;//pointボード見た目切り替え用

	bool startFlag = false;//STRATロゴの出現判定
};
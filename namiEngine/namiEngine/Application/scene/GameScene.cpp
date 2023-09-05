﻿#include "GameScene.h"
#include "FbxLoader.h"
#include "Vector3.h"

#include <cassert>
#include <sstream>
#include <iomanip>
#include "Collision.h"
#include <stdlib.h>
#include <time.h>


using namespace DirectX;

GameScene::GameScene() {
}

GameScene::~GameScene() {
}

void GameScene::Initialize() {
	// nullptrチェック
	dxCommon = DirectXCommon::GetInstance();
	input = Input::GetInstance();
	audio = Audio::GetInstance();

	// カメラ生成
	cameraObject = std::make_unique<Camera>(WinApp::window_width, WinApp::window_height);

	// デバッグテキスト用テクスチャ読み込み
	if (!Sprite::LoadTexture(debugTextTexNumber, L"Resources/debugfont.png")) {
		assert(0);
		return;
	}
	// デバッグテキスト初期化
	debugText = DebugText::GetInstance();
	debugText->Initialize(debugTextTexNumber);

	// テクスチャ読み込み

	//スプライト生成
	//fadeSprite = Sprite::Create(2, { 0.0f,0.0f }, fadeColor);

	//ブロックの模様
	for (int i = 0; i < 3; i++)
	{
		blockSprite[i] = Sprite::Create(i + 1, { i * 64.0f,0.0f });
	}

	cursor = Sprite::Create(4, { 0.0f,0.0f });
	
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			tile[i][j] = Sprite::Create(5, { 128.0f + i * 128.0f, 128.0f + j * 128.0f });
			tile[i][j]->SetSize({ 128.0f,128.0f });
		}
	}

	//数字0～9
	for (int i = 0; i < 10; i++)
	{
		number[i] = Sprite::Create(i + 10, { i * 64.0f, 650.0f });
	}

	//コロン(:)
	colon = Sprite::Create(20, { 0.0f, 740.0f });


	// 背景スプライト生成

	// パーティクルマネージャ生成
	particleMan = ParticleManager::GetInstance();
	particleMan->SetCamera(cameraObject.get());
	particleMan->Clear();

	// カメラ注視点をセット
	cameraObject->SetTarget(cameraTarget);
	cameraObject->SetEye({ cameraEye[0],cameraEye[1],cameraEye[2] });

	// モデル名を指定してファイル読み込み

	// ライト生成
	lightGroup = LightGroup::Create();
	lightGroup->SetCircleShadowActive(0, true);
	// 3Dオブエクトにライトをセット
	Object3d::SetLightGroup(lightGroup.get());
	// 3Dオブジェクトにカメラをセット
	Object3d::SetCamera(cameraObject.get());

	// カメラをセット
	FbxObject3d::SetCamera(cameraObject.get());

	cameraCollider.center = XMLoadFloat3(&cameraObject->GetEye());
	cameraCollider.radius = 5.0f;

	skydomeCollider.center = { 0,0,0 };
	skydomeCollider.radius = 400.0f;

	cameraObject->Update();
	particleMan->Update();
}

void GameScene::Finalize()
{
}

void GameScene::Update() {
	cursor->SetPosition(input->GetMousePosition());
}



void GameScene::Draw() {
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	// 背景スプライト描画
	//spriteBG->Draw();

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon->ClearDepthBuffer();
#pragma endregion

#pragma region 3D描画
	// 3Dオブジェクトの描画
	Object3d::PreDraw(cmdList);

	Object3d::PostDraw();

	particleMan->Draw(cmdList);

	//lightGroup->Draw(cmdList, 3);

	// パーティクルの描画
	//particleMan->Draw(cmdList);
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	//fadeSprite->Draw();

	//ブロックの模様
	for (int i = 0; i < 3; i++)
	{
		blockSprite[i]->Draw();
	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			tile[i][j]->Draw();
		}
	}

	//数字0～9
	for (int i = 0; i < 10; i++)
	{
		number[i]->Draw();
	}

	//コロン
	colon->Draw();

	cursor->Draw();
	// デバッグテキストの描画
	debugText->DrawAll();

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}

#include "ResultScene.h"
#include "FbxLoader.h"
#include "Vector3.h"
#include "StageManager.h"

#include <cassert>
#include <sstream>
#include <iomanip>
#include "Collision.h"
#include <stdlib.h>
#include <time.h>

void ResultScene::Initialize()
{
	// nullptrチェック
	dxCommon = DirectXCommon::GetInstance();
	input = Input::GetInstance();
	audio = Audio::GetInstance();
	scene_ = SceneManager::GetInstance();

	// カメラ生成
	cameraObject = std::make_unique<Camera>(WinApp::window_width, WinApp::window_height);

	srand(time(NULL));//乱数初期化

	// テクスチャ読み込み

	//スプライト生成
	backGround = Sprite::Create(31, { 0.0f,0.0f });

	cursor = Sprite::Create(4, { 0.0f,0.0f });

	//スコア関連
	scoreSprite = Sprite::Create(6, { 588.0f, 359.0f });//SCOREロゴ
	colonScore = Sprite::Create(20, { 752.0f, 359.0f });//コロン(:)
	for (int i = 0; i < 10; i++)
	{
		hunThouScore[i] = Sprite::Create(10 + i, { 782.0f, 359.0f });//[0]00000
	}
	for (int i = 0; i < 10; i++)
	{
		tenThouScore[i] = Sprite::Create(10 + i, { 814.0f, 359.0f });//0[0]0000
	}
	for (int i = 0; i < 10; i++)
	{
		thouScore[i] = Sprite::Create(10 + i, { 846.0f, 359.0f });//00[0]000
	}
	for (int i = 0; i < 10; i++)
	{
		hunScore[i] = Sprite::Create(10 + i, { 878.0f, 359.0f });//000[0]00
	}
	for (int i = 0; i < 10; i++)
	{
		tenScore[i] = Sprite::Create(10 + i, { 910.0f, 359.0f });//0000[0]0
	}
	for (int i = 0; i < 10; i++)
	{
		oneScore[i] = Sprite::Create(10 + i, { 942.0f, 359.0f });//00000[0]
	}

	//リザルト画面関連
	//score
	score = StageManager::GetInstance()->GetScore();
	//RESULTロゴ
	resultSprite = Sprite::Create(21, { 700.0f, 143.0f });
	//RANKロゴ
	rankSprite = Sprite::Create(22, { 613.0f, 525.0f });
	//Aロゴ
	aSprite = Sprite::Create(23, { 900.0f, 525.0f });
	//Bロゴ
	bSprite = Sprite::Create(24, { 900.0f, 525.0f });
	//Cロゴ
	cSprite = Sprite::Create(25, { 900.0f, 525.0f });
	//RETRYロゴ
	retrySprite = Sprite::Create(8, { 312.0f, 715.0f });
	//TITLEロゴ
	titleSprite = Sprite::Create(9, { 1147.0f, 715.0f });

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

void ResultScene::Finalize() {}

void ResultScene::Update()
{
	//ランク表示
	if (80000 <= score)
	{
		rankPattern = 0.0f;
	}
	else if (40000 <= score && score < 80000)
	{
		rankPattern = 1.0f;
	}
	else
	{
		rankPattern = 2.0f;
	}

	XMFLOAT2 retryPos = retrySprite->GetPosition();
	XMFLOAT2 titlePos = titleSprite->GetPosition();
	XMFLOAT2 mousePos = input->GetMousePosition();

	if (retryPos.x < mousePos.x && retryPos.x + 170.0f > mousePos.x && retryPos.y< mousePos.y && retryPos.y + 50.0f > mousePos.y) {
		retrySprite->SetColor(Sprite::Color::GREEN);
		if (input->TriggerMouse(LeftButton)) {
			SceneManager::GetInstance()->ToGameScene();
		}
	}
	else
	{
		retrySprite->SetColor(Sprite::Color::DEF);
	}

	if (titlePos.x < mousePos.x && titlePos.x + 161.0f > mousePos.x && titlePos.y< mousePos.y && titlePos.y + 50.0f > mousePos.y) {
		titleSprite->SetColor(Sprite::Color::GREEN);
		if (input->TriggerMouse(LeftButton)) {
			SceneManager::GetInstance()->ToTitleScene();
		}
	}
	else
	{
		titleSprite->SetColor(Sprite::Color::DEF);
	}

	cursor->SetPosition(input->GetMousePosition());
	if (input->PushMouse(LeftButton)) {
		cursor->SetColor({ 1.0f,1.0f,0.0f,1.0f });
	}
	else {
		cursor->SetColor({ 1.0f,1.0f,1.0f,1.0f });
	}
	//スコア関連
	drawScore[0] = (score / 100000) % 10;//[0]00000
	drawScore[1] = (score / 10000) % 10;//0[0]0000
	drawScore[2] = (score / 1000) % 10;//00[0]000
	drawScore[3] = (score / 100) % 10;//000[0]00
	drawScore[4] = (score / 10) % 10;//0000[0]0
	drawScore[5] = score % 10;//00000[0]
}

void ResultScene::Draw()
{
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

	backGround->Draw();

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

	//スコア関連
	scoreSprite->Draw();//SCOREロゴ
	colonScore->Draw();//コロン(:)
	hunThouScore[drawScore[0]]->Draw();//[0]00000
	tenThouScore[drawScore[1]]->Draw();//0[0]0000
	thouScore[drawScore[2]]->Draw();//00[0]000
	hunScore[drawScore[3]]->Draw();//000[0]00
	tenScore[drawScore[4]]->Draw();//0000[0]0
	oneScore[drawScore[5]]->Draw();//00000[0]

	//リザルト画面関連
	retrySprite->Draw();//RETRYロゴ
	titleSprite->Draw();//TITLEロゴ
	rankSprite->Draw();//RANKロゴ
	if (rankPattern == 0.0f)
	{
		aSprite->Draw();//Aロゴ
	}
	else if (rankPattern == 1.0f)
	{
		bSprite->Draw();//Bロゴ
	}
	else
	{
		cSprite->Draw();//Cロゴ
	}
	resultSprite->Draw();//RESULTロゴ

	cursor->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}
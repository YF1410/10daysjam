#include "NamiGame.h"
#include "GameScene.h"
#include "GameOverScene.h"
#include "TitleScene.h"
#include "TutorialScene.h"
#include "AllLoadSprite.h"

void NamiGame::Initialize() {
	Framework::Initialize();
	scene_ = SceneManager::GetInstance();
	//postEffect = std::make_unique<PostEffect>();
	// スプライト静的初期化
	if (!Sprite::StaticInitialize(WinApp::window_width, WinApp::window_height)) {
		assert(0);
	}
	AllLoadSprite::AllLoadTexture();

	// 3Dオブジェクト静的初期化
	Object3d::StaticInitialize();
	// FBXオブジェクトグラフィックスパイプライン生成
	FbxObject3d::CreateGraphicsPipeline();
	// ライト静的初期化
	LightGroup::StaticInitialize();
	// パーティクルマネージャ初期化
	ParticleManager::GetInstance()->Initialize();
	//FBX初期化
	FbxLoader::GetInstance()->Initialize();

#pragma endregion 汎用機能初期化

	// ゲームシーンの初期化
	scene_->Initialize(new GameOverScene);

	//ポストエフェクト用のテクスチャの読み込み
	//Sprite::LoadTexture(100, L"Resources/white1x1.png");
	//ポストエフェクトの初期化
	//postEffect->Initialize();

}

void NamiGame::Finalize() {
	//解放
	// 各種解放
	FbxLoader::GetInstance()->Finalize();
	ParticleManager::GetInstance()->Finalize();
	LightGroup::StaticFinalize();
	FbxObject3d::StaticFinalize();
	Object3d::StaticFinalize();
	Sprite::StaticFinalize();
	//postEffect.reset();
	Framework::Finalize();
}

void NamiGame::Update() {
	Framework::Update();
	// ゲームシーンの毎フレーム処理
	scene_->Update();
}

void NamiGame::Draw() {
	//レンダーテクスチャへの描画
	//postEffect->PreDrawScene(dxCommon->GetCommandList());
	//gameScene->Draw();
	//postEffect->PostDrawScene(dxCommon->GetCommandList());
#pragma region グラフィックスコマンド
	// 描画開始
	DirectXCommon::GetInstance()->PreDraw();

	//ポストエフェクトの描画
	//postEffect->Draw(dxCommon->GetCommandList());

	// ゲームシーンの描画
	scene_->Draw();

	// 描画終了
	DirectXCommon::GetInstance()->PostDraw();

#pragma endregion グラフィックスコマンド
}
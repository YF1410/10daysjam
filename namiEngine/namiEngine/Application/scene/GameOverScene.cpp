#include "GameOverScene.h"
#include "SceneManager.h"
#include "Easing.h"
#include "AllLoadSprite.h"

void GameOverScene::Initialize()
{
	AllLoadSprite::AllLoadTexture();
	baseBG = Sprite::Create(1, { 0.0f,0.0f });
	gameover = Sprite::Create(46, { 0.0f,0.0f });
	selectToRetry[0] = Sprite::Create(42, {0.0f,0.0f});
	selectToRetry[1] = Sprite::Create(43, { 0.0f,0.0f });
	selectToTitle[0] = Sprite::Create(44, { 0.0f,0.0f });
	selectToTitle[1] = Sprite::Create(45, { 0.0f,0.0f });
	fadeSprite = Sprite::Create(2, { 0.0f,0.0f }, fadeColor);

	dieModel = FbxLoader::GetInstance()->LoadModelFromFile("Die");
	dieObject = std::make_unique<FbxObject3d>();
	dieObject->Initialize();
	dieObject->SetModel(dieModel.get());
	dieObject->SetPosition({ 0.0f,11.0f,-35.0f });
	dieObject->SetRotation({ 0.0f, 180.0f, 0.0f });
	dieObject->PlayAnimation(true);

	cameraObject = std::make_unique<Camera>(WinApp::window_width, WinApp::window_height);
	cameraObject->SetTarget(cameraTarget);
	cameraObject->SetEye({ cameraEye[0],cameraEye[1],cameraEye[2] });
	cameraObject->Update();
	FbxObject3d::SetCamera(cameraObject.get());
}

void GameOverScene::Finalize()
{
}

void GameOverScene::Update()
{
	Input* input = Input::GetInstance();
	if (isFadeIn) {
		fadeColor.w -= 0.04f;
		fadeSprite->SetColor(fadeColor);
		if (fadeColor.w <= 0.0f) {
			isFadeIn = false;
		}
	}

	if (input->TriggerKey(DIK_W)) {
		isRetry = true;
	}
	else if (input->TriggerKey(DIK_S)) {
		isRetry = false;
	}

	if ((input->TriggerKey(DIK_1) || input->TriggerKey(DIK_SPACE) || input->TriggerMouse(MouseButton::LeftButton)) && !isFadeIn) {
		isFadeOut = true;
	}

	SpecifiedMove();

	if (isFadeOut) {
		fadeColor.w += 0.04f;
		fadeSprite->SetColor(fadeColor);
		if (fadeColor.w >= 1.0f) {
			isFadeOut = false;
			if (isRetry) {
				SceneManager::GetInstance()->ToGameScene(StageManager::GetInstance()->GetStageType());
			}
			else if (!isRetry) {
				SceneManager::GetInstance()->ToTitleScene();
			}
		}
	}
	dieObject->Update();
	cameraObject->Update();
}

void GameOverScene::Draw()
{
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();
	// 背景スプライト描画前処理
	Sprite::PreDraw(cmdList);

	baseBG->Draw();
	gameover->Draw();
	// 背景スプライト描画
	if (isRetry) {
		selectToRetry[0]->Draw();
		selectToTitle[1]->Draw();
	}
	else if (!isRetry) {
		selectToRetry[1]->Draw();
		selectToTitle[0]->Draw();
	}
	Sprite::PostDraw();
	// 深度バッファクリア
	DirectXCommon::GetInstance()->ClearDepthBuffer();


	dieObject->Draw(cmdList);

	Sprite::PreDraw(cmdList);
	fadeSprite->Draw();
	// スプライト描画後処理
	Sprite::PostDraw();
}

void GameOverScene::SpecifiedMove()
{
	if (maxSpecifiedMoveTimer >= specifiedMoveTimer) {
		specifiedMoveTimer++;
		if (maxSpecifiedMoveTimer <= specifiedMoveTimer) {
			specifiedMoveTimer = 0;
			if (!isUp) {
				isUp = true;
			}
			else if (isUp) {
				isUp = false;
			}
		}
	}

	float eTime = (float)(specifiedMoveTimer / static_cast<double>(maxSpecifiedMoveTimer));

	if (isUp) {
		gameOverObjectPosition = static_cast<float>(Ease(In, ease::Quint, eTime, specifiedBouncePosUp, specifiedBouncePosDown));
	}
	else if (!isUp) {
		gameOverObjectPosition = static_cast<float>(Ease(Out, ease::Quint, eTime, specifiedBouncePosDown, specifiedBouncePosUp));
	}

	if (isRetry) {
		gameover->SetPosition({ 0,gameOverObjectPosition });
		selectToRetry[0]->SetPosition({ 0,gameOverObjectPosition });
		selectToTitle[1]->SetPosition({ 0,defaultObjectPosition });
	}
	else if (!isRetry) {
		gameover->SetPosition({ 0,gameOverObjectPosition });
		selectToRetry[1]->SetPosition({ 0,defaultObjectPosition });
		selectToTitle[0]->SetPosition({ 0,gameOverObjectPosition });
	}
}
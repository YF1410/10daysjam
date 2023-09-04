#include "TitleScene.h"
#include "SceneManager.h"
#include "AllLoadSprite.h"

void TitleScene::Initialize()
{
	titleBG = Sprite::Create(1, { 0.0f,0.0f });
	fadeSprite = Sprite::Create(2, { 0.0f,0.0f }, fadeColor);
	tutorial[0] = Sprite::Create(3, { 0.0f,0.0f });
	tutorial[1] = Sprite::Create(4, { 0.0f,0.0f });
	title[0] = Sprite::Create(5, { 0.0f,0.0f });
	title[1] = Sprite::Create(6, { 0.0f,0.0f });
}

void TitleScene::Finalize()
{
}

void TitleScene::Update()
{
	if (isFadeIn) {
		fadeColor.w -= 0.05f;
		fadeSprite->SetColor(fadeColor);
		if (fadeColor.w <= 0.0f) {
			fadeColor.w = 0.0f;
			isFadeIn = false;
		}
	}

	if ((Input::GetInstance()->TriggerKey(DIK_1) || Input::GetInstance()->TriggerKey(DIK_SPACE)
		|| Input::GetInstance()->TriggerMouse(MouseButton::LeftButton)) && !isFadeIn) {

		if (tutorialNum == 1) {
			isFadeOut = true;
		}
		else if (isTutorial) {
			tutorialNum++;
		}

		isTutorial = true;
	}

	SpecifiedMove();

	if (isFadeOut) {
		fadeColor.w += 0.04f;
		fadeSprite->SetColor(fadeColor);
		if (fadeColor.w >= 1.0f) {
			isFadeOut = false;
			SceneManager::GetInstance()->ToGameScene(StageManager::firstStage);
			//SceneManager::GetInstance()->ToTutorialScene();
		}
	}
}

void TitleScene::Draw()
{
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();
	// 背景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	// 背景スプライト描画
	titleBG->Draw();
	title[0]->Draw();
	title[1]->Draw();
	if (isTutorial) {
		tutorial[tutorialNum]->Draw();
	}
	if (isFadeOut) {
		fadeSprite->Draw();
	}

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	DirectXCommon::GetInstance()->ClearDepthBuffer();
}

void TitleScene::SpecifiedMove()
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
		titleObjectPosition = static_cast<float>(Ease(In, ease::Quint, eTime, specifiedBouncePosUp, specifiedBouncePosDown));
		startObjectPosition = static_cast<float>(Ease(In, ease::Quint, eTime, specifiedBounceStartPosUp, specifiedBounceStartPosDown));
	}
	else if (!isUp) {
		titleObjectPosition = static_cast<float>(Ease(Out, ease::Quint, eTime, specifiedBouncePosDown, specifiedBouncePosUp));
		startObjectPosition = static_cast<float>(Ease(Out, ease::Quint, eTime, specifiedBounceStartPosDown, specifiedBounceStartPosUp));
	}

	title[0]->SetPosition({ 0,titleObjectPosition });
	title[1]->SetPosition({ 0,startObjectPosition });
}


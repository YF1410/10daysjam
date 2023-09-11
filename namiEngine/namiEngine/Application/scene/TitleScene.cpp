#include "TitleScene.h"
#include "FbxLoader.h"
#include "Vector3.h"

#include <cassert>
#include <sstream>
#include <iomanip>
#include "Collision.h"
#include <stdlib.h>
#include <time.h>

void TitleScene::Initialize()
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
	backGround = Sprite::Create(36, { 0.0f,0.0f });

	cursor = Sprite::Create(4, { 0.0f,0.0f });

	pointCircle = Sprite::Create(33, { 96.0f,320.0f });
	pointTriangle = Sprite::Create(34, { 96.0f,320.0f });
	pointSquare = Sprite::Create(35, { 96.0f,320.0f });
	clickDrag = Sprite::Create(32, { 1070.0f,300.0f });
	startSprite = Sprite::Create(27, { 727.0f,794.0f });

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			tile[j][i] = Sprite::Create(5, { 544.0f + i * tileSize, 194.0f + j * tileSize });
			tile[j][i]->SetSize({ tileSize,tileSize });
			tile[j][i]->SetColor(Sprite::Color::DEF);
		}
	}

	//画面左のブロック(〇)
	for (int i = 0; i < 4; i++) {
		for (int j = 4; j < 8; j++) {
			float tileSize = 128.0f;
			circle[j][i] = Sprite::Create(1, { 544.0f + i * tileSize, 194.0f + (j - 4) * tileSize });
			circle[j][i]->SetSize({ tileSize,tileSize });
		}
	}

	//画面左のブロック(△)
	for (int i = 0; i < 4; i++) {
		for (int j = 4; j < 8; j++) {
			float tileSize = 128.0f;
			triangle[j][i] = Sprite::Create(2, { 544.0f + i * tileSize, 194.0f + (j - 4) * tileSize });
			triangle[j][i]->SetSize({ tileSize,tileSize });
		}
	}

	//画面左のブロック(♢)
	for (int i = 0; i < 4; i++) {
		for (int j = 4; j < 8; j++) {
			float tileSize = 128.0f;
			square[j][i] = Sprite::Create(3, { 544.0f + i * tileSize, 194.0f + (j - 4) * tileSize });
			square[j][i]->SetSize({ tileSize,tileSize });
		}
	}

	//SCOREロゴ
	scoreSprite = Sprite::Create(6, { 1000.0f, 100.0f });

	//スコア関連
	scoreSprite = Sprite::Create(6, { 1153.0f, 100.0f });//SCOREロゴ
	colonScore = Sprite::Create(20, { 1317.0f, 100.0f });//コロン(:)
	for (int i = 0; i < 10; i++)
	{
		hunThouScore[i] = Sprite::Create(10 + i, { 1347.0f, 100.0f });//[0]00000
	}
	for (int i = 0; i < 10; i++)
	{
		tenThouScore[i] = Sprite::Create(10 + i, { 1379.0f, 100.0f });//0[0]0000
	}
	for (int i = 0; i < 10; i++)
	{
		thouScore[i] = Sprite::Create(10 + i, { 1411.0f, 100.0f });//00[0]000
	}
	for (int i = 0; i < 10; i++)
	{
		hunScore[i] = Sprite::Create(10 + i, { 1443.0f, 100.0f });//000[0]00
	}
	for (int i = 0; i < 10; i++)
	{
		tenScore[i] = Sprite::Create(10 + i, { 1475.0f, 100.0f });//0000[0]0
	}
	for (int i = 0; i < 10; i++)
	{
		oneScore[i] = Sprite::Create(10 + i, { 1507.0f, 100.0f });//00000[0]
	}

	//ブロック表示関連初期化
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			tilePattern[j][i] = 0.0f;//画面左のブロック配置(0:なし 1:〇 2:△ 3:♢)

			circleArive[j][i] = false;//画面左の〇の表示(0:非表示 1:表示)
			triangleArive[j][i] = false;//画面左の△の表示(0:非表示 1:表示)
			squareArive[j][i] = false;//画面左の♢の表示(0:非表示 1:表示)


			//左右のタイルにランダムでブロック配置
			tilePattern[j][i] = (rand() % 3) + 1;

		}
	}

	for (int i = 0; i < 10; i++)
	{
		breakPattern[i] = 0.0f;//消すブロックの種類
	}

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

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (tilePattern[j][i] == 1.0f)
			{
				circleArive[j][i] = true;
			}
			if (tilePattern[j][i] == 2.0f)
			{
				triangleArive[j][i] = true;
			}
			if (tilePattern[j][i] == 3.0f)
			{
				squareArive[j][i] = true;
			}

		}
	}
}

void TitleScene::Finalize() {}

void TitleScene::Update()
{
	XMFLOAT2 startPos = startSprite->GetPosition();
	XMFLOAT2 mousePos = input->GetMousePosition();

	if (startPos.x < mousePos.x && startPos.x + 177.0f > mousePos.x && startPos.y< mousePos.y && startPos.y + 50.0f > mousePos.y) {
		startSprite->SetColor(Sprite::Color::GREEN);
		if (input->TriggerMouse(LeftButton)) {
			SceneManager::GetInstance()->ToGameScene();
		}
	}
	else
	{
		startSprite->SetColor(Sprite::Color::DEF);
	}

	cursor->SetPosition(input->GetMousePosition());
	GetScore();//スコア獲得処理
	ScoreCharge();//スコア加算処理

	//スコア関連
	drawScore[0] = (score / 100000) % 10;//[0]00000
	drawScore[1] = (score / 10000) % 10;//0[0]0000
	drawScore[2] = (score / 1000) % 10;//00[0]000
	drawScore[3] = (score / 100) % 10;//000[0]00
	drawScore[4] = (score / 10) % 10;//0000[0]0
	drawScore[5] = score % 10;//00000[0]

	//ポイントボード
	pointChange++;
	if (pointChange >= 60.0f)
	{
		if (pointPattern == 2.0f)
		{
			pointPattern = 0.0f;
		}
		else
		{
			pointPattern += 1.0f;
		}
		pointChange = 0.0f;
	}

	//マウスカーソル
	if (input->TriggerMouse(LeftButton)) {
		saveCursorPos = cursor->GetPosition();
	}
	if (input->PushMouse(LeftButton)) {
		cursor->SetColor({ 1.0f,1.0f,0.0f,1.0f });
	}
	else {
		cursor->SetColor({ 1.0f,1.0f,1.0f,1.0f });
	}
	if (!input->PushMouse(LeftButton)) {
		xFlag = false;
		yFlag = false;
		for (int i = 0; i < 4; i++) {
			for (int j = 4; j < 8; j++) {
				if (tile[j - 4][i]->GeteColor() == Sprite::Color::RED) {
					GetBreakPattern(tilePattern[j][i]);
					circleArive[j][i] = false;
					triangleArive[j][i] = false;
					squareArive[j][i] = false;
					yFlag = false;
				}
				if (tile[j - 4][i]->GeteColor() == Sprite::Color::BLUE) {
					GetBreakPattern(tilePattern[j][i]);
					circleArive[j][i] = false;
					triangleArive[j][i] = false;
					squareArive[j][i] = false;
					yFlag = false;
				}
				tile[j - 4][i]->SetColor(Sprite::Color::DEF);
			}
		}

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 8; j++) {
				if (!circleArive[j][i] && !triangleArive[j][i] && !squareArive[j][i]) {
					if (tilePattern[j - 1][i] == 1.0f) {
						circleArive[j][i] = true;
						tilePattern[j][i] = 1.0f;
						circleArive[j - 1][i] = false;
						triangleArive[j - 1][i] = false;
						squareArive[j - 1][i] = false;
					}
					if (tilePattern[j - 1][i] == 2.0f) {
						triangleArive[j][i] = true;
						tilePattern[j][i] = 2.0f;
						circleArive[j - 1][i] = false;
						triangleArive[j - 1][i] = false;
						squareArive[j - 1][i] = false;
					}
					if (tilePattern[j - 1][i] == 3.0f) {
						squareArive[j][i] = true;
						tilePattern[j][i] = 3.0f;
						circleArive[j - 1][i] = false;
						triangleArive[j - 1][i] = false;
						squareArive[j - 1][i] = false;
					}

					if (!circleArive[0][i] && !triangleArive[0][i] && !squareArive[0][i])
						tilePattern[0][i] = (rand() % 3) + 1;
					if (tilePattern[0][i] == 1.0f) {
						circleArive[0][i] = true;
						tilePattern[0][i] = 1.0f;

					}
					if (tilePattern[0][i] == 2.0f) {
						triangleArive[0][i] = true;
						tilePattern[0][i] = 2.0f;

					}
					if (tilePattern[0][i] == 3.0f) {
						squareArive[0][i] = true;
						tilePattern[0][i] = 3.0f;

					}
				}
			}
		}
	}

	XMFLOAT2 cursorPos = cursor->GetPosition();

	if (input->PushMouse(LeftButton)) {
		//横に動かしたか縦に動かしたかの確認
		float a = saveCursorPos.x - cursorPos.x;
		float b = cursorPos.x - saveCursorPos.x;
		float c = saveCursorPos.y - cursorPos.y;
		float d = cursorPos.y - saveCursorPos.y;
		float xTmp = 0.0f;
		float yTmp = 0.0f;

		if (a < b) {
			xTmp = a;
			a = b;
			b = xTmp;
		}

		if (c < d) {
			yTmp = c;
			c = d;
			d = yTmp;
		}

		if (a > 20.0f && !yFlag) {
			xFlag = true;
		}
		if (c > 20.0f && !xFlag) {
			yFlag = true;
		}

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {

				XMFLOAT2 tilePos = tile[j][i]->GetPosition();
				if (input->PushMouse(LeftButton)) {
					if (a > c && !yFlag) {
						//x軸処理
						//xFlag = true;

						if (saveCursorPos.x < cursorPos.x && saveCursorPos.x < tilePos.x + tileSize && tilePos.x < cursorPos.x) {
							if (tilePos.y < saveCursorPos.y && tilePos.y + tileSize > saveCursorPos.y) {
								tile[j][i]->SetColor(Sprite::Color::RED);
							}
						}
						else if (saveCursorPos.x > cursorPos.x && saveCursorPos.x > tilePos.x && tilePos.x + tileSize > cursorPos.x) {
							if (tilePos.y < saveCursorPos.y && tilePos.y + tileSize > saveCursorPos.y) {
								tile[j][i]->SetColor(Sprite::Color::RED);
							}
						}
					}

					else if (a < c && !xFlag) {
						//y軸処理
						//yFlag = true;

						if (saveCursorPos.y < cursorPos.y && saveCursorPos.y < tilePos.y + tileSize && tilePos.y < cursorPos.y) {
							if (tilePos.x < saveCursorPos.x && tilePos.x + tileSize > saveCursorPos.x) {
								tile[j][i]->SetColor(Sprite::Color::BLUE);
							}
						}
						else if (saveCursorPos.y > cursorPos.y && saveCursorPos.y > tilePos.y && tilePos.y + tileSize > cursorPos.y) {
							if (tilePos.x < saveCursorPos.x && tilePos.x + tileSize > saveCursorPos.x) {
								tile[j][i]->SetColor(Sprite::Color::BLUE);
							}
						}
					}
				}
			}
		}
	}
}

void TitleScene::Draw()
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

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			tile[j][i]->Draw();
		}
	}

	//画面左のブロック(〇)
	for (int i = 0; i < 4; i++) {
		for (int j = 4; j < 8; j++) {
			if (circleArive[j][i]) {
				circle[j][i]->Draw();
			}
		}
	}

	//画面左のブロック(△)
	for (int i = 0; i < 4; i++) {
		for (int j = 4; j < 8; j++) {
			if (triangleArive[j][i]) {
				triangle[j][i]->Draw();
			}
		}
	}

	//画面左のブロック(♢)
	for (int i = 0; i < 4; i++) {
		for (int j = 4; j < 8; j++) {
			if (squareArive[j][i]) {
				square[j][i]->Draw();
			}
		}
	}

	//スコア関連
	scoreSprite->Draw();//SCOREロゴ
	colonScore->Draw();//コロン(:)
	hunThouScore[drawScore[0]]->Draw();//[0]00000
	tenThouScore[drawScore[1]]->Draw();//0[0]0000
	thouScore[drawScore[2]]->Draw();//00[0]000
	hunScore[drawScore[3]]->Draw();//000[0]00
	tenScore[drawScore[4]]->Draw();//0000[0]0
	oneScore[drawScore[5]]->Draw();//00000[0]

	if (pointPattern == 0.0f)
	{
		pointCircle->Draw();//Point〇
	}
	else if (pointPattern == 1.0f)
	{
		pointTriangle->Draw();//Point△
	}
	else if (pointPattern == 2.0f)
	{
		pointSquare->Draw();//Point♢
	}

	clickDrag->Draw();//ClickDrag
	startSprite->Draw();//STARTロゴ

	cursor->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}

void TitleScene::GetBreakPattern(float block)
{
	//パターンを保存
	for (int i = 0; i < 4; i++)
	{
		if (breakPattern[i] == 0.0f)
		{
			breakPattern[i] = block;
			break;
		}
	}
}

void TitleScene::GetScore()
{
	//何かしらのパターンが入っていたら処理開始
	if (breakPattern[0] != 0.0f)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 1; j < 4; j++)
			{
				//4つすべて同じ模様
				if (breakPattern[i] == j && breakPattern[i + 1] == j && breakPattern[i + 2] == j && breakPattern[i + 3] == j)
				{
					preScore += 1000;
					break;
				}
				//3つ同じ模様
				else if (breakPattern[i] == j && breakPattern[i + 1] == j && breakPattern[i + 2] == j)
				{
					preScore += 400;
					break;
				}
				//2つ同じ模様
				else if (breakPattern[i] == j && breakPattern[i + 1] == j)
				{
					preScore += 200;
					break;
				}
			}
		}
		//保存したパターンを削除
		for (int i = 0; i < 4; i++)
		{
			breakPattern[i] = 0.0f;
		}
	}
}

void TitleScene::ScoreCharge()
{
	//加算前のスコアが1以上かつ加算後のスコアが999999未満なら実行
	if (preScore > 0 && score < 999999)
	{
		if (preScore > 20)
		{
			preScore -= 15;//-15
			score += 15;//+15
		}
		else
		{
			preScore -= 1;//-1
			score += 1;//+1
		}
	}
}
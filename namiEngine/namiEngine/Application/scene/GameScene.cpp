#include "GameScene.h"
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

	srand(time(NULL));//乱数初期化

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
			tile[i][j] = Sprite::Create(5, { 200.0f + i * tileSize, 250.0f + j * tileSize });
			tile[i][j]->SetSize({ tileSize,tileSize });
		}
	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			float subTileSize = 80.0f;
			subTile[i][j] = Sprite::Create(5, { 1110.0f + i * subTileSize, 350.0f + j * subTileSize });
			subTile[i][j]->SetSize({ subTileSize,subTileSize });
		}
	}

	//画面左のブロック(〇)
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			float tileSize = 128.0f;
			circle[i][j] = Sprite::Create(1, { 200.0f + i * tileSize, 250.0f + j * tileSize });
			circle[i][j]->SetSize({ tileSize,tileSize });
		}
	}

	//画面左のブロック(△)
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			float tileSize = 128.0f;
			triangle[i][j] = Sprite::Create(2, { 200.0f + i * tileSize, 250.0f + j * tileSize });
			triangle[i][j]->SetSize({ tileSize,tileSize });
		}
	}

	//画面左のブロック(♢)
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			float tileSize = 128.0f;
			square[i][j] = Sprite::Create(3, { 200.0f + i * tileSize, 250.0f + j * tileSize });
			square[i][j]->SetSize({ tileSize,tileSize });
		}
	}

	//画面右のブロック(〇)
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			float subTileSize = 80.0f;
			subCircle[i][j] = Sprite::Create(1, { 1110.0f + i * subTileSize, 350.0f + j * subTileSize });
			subCircle[i][j]->SetSize({ subTileSize,subTileSize });
		}
	}

	//画面右のブロック(△)
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			float subTileSize = 80.0f;
			subTriangle[i][j] = Sprite::Create(2, { 1110.0f + i * subTileSize, 350.0f + j * subTileSize });
			subTriangle[i][j]->SetSize({ subTileSize,subTileSize });
		}
	}

	//画面右のブロック(♢)
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			float subTileSize = 80.0f;
			subSquare[i][j] = Sprite::Create(3, { 1110.0f + i * subTileSize, 350.0f + j * subTileSize });
			subSquare[i][j]->SetSize({ subTileSize,subTileSize });
		}
	}

	//数字0～9
	for (int i = 0; i < 10; i++)
	{
		number[i] = Sprite::Create(i + 10, { (i * 35.0f) + 1200.0f, 100.0f });
	}

	//SCOREロゴ
	scoreSprite = Sprite::Create(6, { 1000.0f, 100.0f });

	//NEXTロゴ
	nextSprite = Sprite::Create(7, { 1200.0f, 250.0f });

	//制限時間関連
	timeSprite = Sprite::Create(26, { 330.0f, 80.0f });//TIMEロゴ
	colonTime = Sprite::Create(20, { 460.0f, 80.0f });//コロン(:)
	for (int i = 0; i < 10; i++)
	{
		hunSecTime[i] = Sprite::Create(10 + i, { 495.0f, 80.0f });//[1]20
	}
	for (int i = 0; i < 10; i++)
	{
		tenSecTime[i] = Sprite::Create(10 + i, { 530.0f, 80.0f });//1[2]0
	}
	for (int i = 0; i < 10; i++)
	{
		oneSecTime[i] = Sprite::Create(10 + i, { 565.0f, 80.0f });//12[0]
	}

	//スコア関連
	scoreSprite = Sprite::Create(6, { 1070.0f, 100.0f });//SCOREロゴ
	colonScore = Sprite::Create(20, { 1235.0f, 100.0f });//コロン(:)
	for (int i = 0; i < 10; i++)
	{
		hunThouScore[i] = Sprite::Create(10 + i, { 1270.0f, 100.0f });//[0]00000
	}
	for (int i = 0; i < 10; i++)
	{
		tenThouScore[i] = Sprite::Create(10 + i, { 1305.0f, 100.0f });//0[0]0000
	}
	for (int i = 0; i < 10; i++)
	{
		thouScore[i] = Sprite::Create(10 + i, { 1340.0f, 100.0f });//00[0]000
	}
	for (int i = 0; i < 10; i++)
	{
		hunScore[i] = Sprite::Create(10 + i, { 1375.0f, 100.0f });//000[0]00
	}
	for (int i = 0; i < 10; i++)
	{
		tenScore[i] = Sprite::Create(10 + i, { 1410.0f, 100.0f });//0000[0]0
	}
	for (int i = 0; i < 10; i++)
	{
		oneScore[i] = Sprite::Create(10 + i, { 1445.0f, 100.0f });//00000[0]
	}

	//ブロック表示関連初期化
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			tilePattern[i][j] = 0.0f;//画面左のブロック配置(0:なし 1:〇 2:△ 3:♢)
			subTilePattern[i][j] = 0.0f;//画面右のブロック配置(0:なし 1:〇 2:△ 3:♢)
			circleArive[i][j] = false;//画面左の〇の表示(0:非表示 1:表示)
			triangleArive[i][j] = false;//画面左の△の表示(0:非表示 1:表示)
			squareArive[i][j] = false;//画面左の♢の表示(0:非表示 1:表示)
			subCircleArive[i][j] = false;//画面右の〇の表示(0:非表示 1:表示)
			subTriangleArive[i][j] = false;//画面右の△の表示(0:非表示 1:表示)
			subSquareArive[i][j] = false;//画面右の〇の表示(0:非表示 1:表示)

			//左右のタイルにランダムでブロック配置
			tilePattern[i][j] = (rand() % 3) + 1;
			subTilePattern[i][j] = (rand() % 3) + 1;
		}
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
}

void GameScene::Finalize()
{
}

void GameScene::Update() {
	cursor->SetPosition(input->GetMousePosition());

	//制限時間関連
	if (gameTime > 0) { gameTime--; }//制限時間を減らす
	calculatedTime = gameTime / 60;//制限時間表示用に計算
	drawTime[0] = (calculatedTime / 100) % 10;//[1]00
	drawTime[1] = (calculatedTime / 10) % 10;//0[1]0
	drawTime[2] = calculatedTime % 10;//00[1]

	//スコア関連
	drawScore[0] = (score / 100000) % 10;//[0]00000
	drawScore[1] = (score / 10000) % 10;//0[0]0000
	drawScore[2] = (score / 1000) % 10;//00[0]000
	drawScore[3] = (score / 100) % 10;//000[0]00
	drawScore[4] = (score / 10) % 10;//0000[0]0
	drawScore[5] = score % 10;//00000[0]

	//ブロック配置関連
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (tilePattern[i][j] == 1.0f)
			{
				circleArive[i][j] = true;
			}
			if (tilePattern[i][j] == 2.0f)
			{
				triangleArive[i][j] = true;
			}
			if (tilePattern[i][j] == 3.0f)
			{
				squareArive[i][j] = true;
			}
			if (subTilePattern[i][j] == 1.0f)
			{
				subCircleArive[i][j] = true;
			}
			if (subTilePattern[i][j] == 2.0f)
			{
				subTriangleArive[i][j] = true;
			}
			if (subTilePattern[i][j] == 3.0f)
			{
				subSquareArive[i][j] = true;
			}
		}
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
			for (int j = 0; j < 4; j++) {

				tile[i][j]->SetColor({ 1.0f,1.0f,1.0f,1.0f });
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

		if (a > 100.0f && !yFlag) {
			xFlag = true;
		}
		if (c > 100.0f && !xFlag) {
			yFlag = true;
		}

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				
				XMFLOAT2 tilePos = tile[i][j]->GetPosition();

				if (a > c && !yFlag) {
					//x軸処理
					//xFlag = true;
					if (tilePos.x < cursorPos.x && tilePos.x + tileSize > cursorPos.x) {
						if (tilePos.y < saveCursorPos.y && tilePos.y + tileSize > saveCursorPos.y) {
							if (input->PushMouse(LeftButton)) {
								tile[i][j]->SetColor({ 1.0f,0.0f,0.0f,1.0f });
							}
						}
					}

				}
				else if (a < c && !xFlag) {
					//y軸処理
					//yFlag = true;
					if (tilePos.y < cursorPos.y && tilePos.y + tileSize > cursorPos.y) {
						if (tilePos.x < saveCursorPos.x && tilePos.x + tileSize > saveCursorPos.x) {
							if (input->PushMouse(LeftButton)) {
								tile[i][j]->SetColor({ 0.0f,0.0f,1.0f,1.0f });
							}
						}
					}

				}
			}



			//x軸の判定
			/*if (saveCursorPos.x < cursorPos.x) {
				if (saveCursorPos.x - cursorPos.x < saveCursorPos.y - cursorPos.y || saveCursorPos.x - cursorPos.x < (saveCursorPos.y - cursorPos.y) * -1) {
					if (tilePos.x < cursorPos.x && tilePos.x + tileSize > cursorPos.x) {
						if (tilePos.y < saveCursorPos.y && tilePos.y + tileSize > saveCursorPos.y) {
							if (input->PushMouse(LeftButton)) {
								tile[i][j]->SetColor({ 1.0f,0.0f,0.0f,1.0f });
							}
						}
					}
				}
			}
			else if (saveCursorPos.x > cursorPos.x) {
				if (cursorPos.x - saveCursorPos.x < cursorPos.y - saveCursorPos.y || cursorPos.x - saveCursorPos.x < (cursorPos.y - saveCursorPos.y) * -1) {
					if (tilePos.x < cursorPos.x && tilePos.x + tileSize > cursorPos.x) {
						if (tilePos.y < saveCursorPos.y && tilePos.y + tileSize > saveCursorPos.y) {
							if (input->PushMouse(LeftButton)) {
								tile[i][j]->SetColor({ 1.0f,0.0f,0.0f,1.0f });
							}
						}
					}
				}
			}*/

			/*if (saveCursorPos.y < cursorPos.y) {
				if (saveCursorPos.y - cursorPos.y < saveCursorPos.x - cursorPos.x || saveCursorPos.y - cursorPos.y < (saveCursorPos.x - cursorPos.x) * -1) {
					if (tilePos.y < cursorPos.y && tilePos.y + tileSize > cursorPos.y) {
						if (tilePos.x < saveCursorPos.x && tilePos.x + tileSize > saveCursorPos.x) {
							if (input->PushMouse(LeftButton)) {
								tile[i][j]->SetColor({ 1.0f,0.0f,0.0f,1.0f });
							}
						}
					}
				}
			}
			else if (saveCursorPos.y > cursorPos.y) {
				if (cursorPos.y - saveCursorPos.y < cursorPos.x - saveCursorPos.x || cursorPos.y - saveCursorPos.y < (cursorPos.x - saveCursorPos.x) * -1) {
					if (tilePos.y < cursorPos.y && tilePos.y + tileSize > cursorPos.y) {
						if (tilePos.x < saveCursorPos.x && tilePos.x + tileSize > saveCursorPos.x) {
							if (input->PushMouse(LeftButton)) {
								tile[i][j]->SetColor({ 0.0f,0.0f,1.0f,1.0f });
							}
						}
					}
				}
			}*/

			/*if (saveCursorPos.x - cursorPos.x > saveCursorPos.y - cursorPos.y || cursorPos.x - saveCursorPos.x > cursorPos.y - saveCursorPos.y) {
				if (tilePos.x < cursorPos.x && tilePos.x + tileSize > cursorPos.x) {
					if (tilePos.y < saveCursorPos.y && tilePos.y + tileSize > saveCursorPos.y) {
						if (input->PushMouse(LeftButton)) {
							tile[i][j]->SetColor({ 1.0f,0.0f,0.0f,1.0f });
						}
					}
				}
			}
			if (saveCursorPos.x - cursorPos.x < saveCursorPos.y - cursorPos.y || cursorPos.x - saveCursorPos.x < cursorPos.y - saveCursorPos.y) {
				if (tilePos.y < cursorPos.y && tilePos.y + tileSize > cursorPos.y) {
					if (tilePos.x < saveCursorPos.x && tilePos.x + tileSize > saveCursorPos.x) {
						if (input->PushMouse(LeftButton)) {
							tile[i][j]->SetColor({ 0.0f,0.0f,1.0f,1.0f });
						}
					}
				}
			}*/
		}
	}

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

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			subTile[i][j]->Draw();
		}
	}

	//画面左のブロック(〇)
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (circleArive[i][j]) {
				circle[i][j]->Draw();
			}
		}
	}

	//画面左のブロック(△)
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (triangleArive[i][j]) {
				triangle[i][j]->Draw();
			}
		}
	}

	//画面左のブロック(♢)
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (squareArive[i][j]) {
				square[i][j]->Draw();
			}
		}
	}

	//画面右のブロック(〇)
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (subCircleArive[i][j]) {
				subCircle[i][j]->Draw();
			}
		}
	}

	//画面右のブロック(△)
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (subTriangleArive[i][j]) {
				subTriangle[i][j]->Draw();
			}
		}
	}

	//画面右のブロック(♢)
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (subSquareArive[i][j]) {
				subSquare[i][j]->Draw();
			}
		}
	}

	//数字0～9
	for (int i = 0; i < 10; i++)
	{
		//number[i]->Draw();
	}

	//NEXTロゴ
	nextSprite->Draw();

	//制限時間関連
	timeSprite->Draw();//TIMEロゴ
	colonTime->Draw();//コロン(:)
	hunSecTime[drawTime[0]]->Draw();//[1]20
	tenSecTime[drawTime[1]]->Draw();//1[2]0
	oneSecTime[drawTime[2]]->Draw();//12[0]

	//スコア関連
	scoreSprite->Draw();//SCOREロゴ
	colonScore->Draw();//コロン(:)
	hunThouScore[drawScore[0]]->Draw();//[0]00000
	tenThouScore[drawScore[1]]->Draw();//0[0]0000
	thouScore[drawScore[2]]->Draw();//00[0]000
	hunScore[drawScore[3]]->Draw();//000[0]00
	tenScore[drawScore[4]]->Draw();//0000[0]0
	oneScore[drawScore[5]]->Draw();//00000[0]

	cursor->Draw();
	// デバッグテキストの描画
	debugText->DrawAll();

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}

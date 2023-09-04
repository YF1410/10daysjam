#include "Framework.h"

void Framework::Run()
{
	Initialize();

	while (true)  // ゲームループ
	{
		Update();

		//終了処理が入ったら
		if (isEnd_) {
			break;
		}

		Draw();
	}

	Finalize();
}

void Framework::Initialize() {
	win = std::make_unique<WinApp>();
	// ゲームウィンドウの作成
	win->CreateGameWindow();

	//DirectX初期化処理
	DirectXCommon::GetInstance()->Initialize(win->GetHwnd());

	//入力の初期化
	Input::GetInstance()->Initialize(win->GetInstance(), win->GetHwnd());

	// オーディオの初期化
	if (!Audio::GetInstance()->Initialize()) {
		assert(0);
	}
}

void Framework::Finalize() {
	// ゲームウィンドウの破棄
	win->TerminateGameWindow();
}

void Framework::Update() {
	// メッセージ処理
	if (win->ProcessMessage() || Input::GetInstance()->TriggerKey(DIK_ESCAPE)) {
		isEnd_ = true;
		return;
	}

	// 入力関連の毎フレーム処理
	Input::GetInstance()->Update();
}
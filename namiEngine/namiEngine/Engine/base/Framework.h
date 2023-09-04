#pragma once
#include "WinApp.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Audio.h"
class Framework
{
public:
	//実行
	void Run();
	//初期化
	virtual void Initialize();
	//終了
	virtual void Finalize();
	//毎フレーム更新
	virtual void Update();
	//描画
	virtual void Draw() = 0;
protected:
	bool isEnd_ = false;
	std::unique_ptr<WinApp> win;
};


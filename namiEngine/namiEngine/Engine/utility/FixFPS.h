#pragma once
#include <chrono>

class FixFPS
{
public:
	//初期化
	void Initialize();
	//更新
	void Update();

private:
	std::chrono::steady_clock::time_point reference_;
};


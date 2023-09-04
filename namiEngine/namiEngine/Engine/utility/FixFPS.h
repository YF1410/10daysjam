#pragma once
#include <chrono>

class FixFPS
{
public:
	//‰Šú‰»
	void Initialize();
	//XV
	void Update();

private:
	std::chrono::steady_clock::time_point reference_;
};


﻿#include "NamiGame.h"

//# Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	std::unique_ptr<Framework> game = std::make_unique<NamiGame>();

	game->Run();

	return 0;
}
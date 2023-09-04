#pragma once

#include "Input.h"
#include "Sprite.h"
#include "SceneInterface.h"
#include "FbxObject3d.h"

#include <memory>

class GameOverScene : public SceneInterface
{
public:
	void Initialize();
	void Finalize();
	void Update();
	void Draw();
	void SpecifiedMove();
private:
	std::unique_ptr<Sprite> baseBG;
	std::unique_ptr<Sprite> gameover;
	std::unique_ptr<Sprite> selectToRetry[2];
	std::unique_ptr<Sprite> selectToTitle[2];
	std::unique_ptr<Sprite> fadeSprite;
	XMFLOAT4 fadeColor = { 1.0f,1.0f,1.0f,1.0f };
	bool isFadeOut = false;
	bool isFadeIn = true;
	bool isRetry = true;
	std::unique_ptr<FbxModel> dieModel;
	std::unique_ptr<FbxObject3d> dieObject;
	std::unique_ptr<Camera> cameraObject;
	float cameraEye[3] = { 0.0f,0.0f,-10.0f };
	XMFLOAT3 cameraTarget = { 0.0f,0.0f,0.0f };

	float gameOverObjectPosition = 0;
	float defaultObjectPosition = 0;

	bool isUp = false;
	int specifiedMoveTimer = 0;
	int maxSpecifiedMoveTimer = 15;
	float bounceAmount = 20.0f;
	float specifiedBouncePosUp = 0 + bounceAmount;
	float specifiedBouncePosDown = 0 - bounceAmount;
};


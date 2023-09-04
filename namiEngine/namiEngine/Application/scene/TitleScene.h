#pragma once

#include "Input.h"
#include "Sprite.h"
#include "SceneInterface.h"
#include "Easing.h"

#include <memory>
class TitleScene:public SceneInterface
{
public:
	void Initialize()override;
	void Finalize()override;
	void Update()override;
	void Draw()override;
	void SpecifiedMove();
private:
	std::unique_ptr<Sprite> titleBG;
	std::unique_ptr<Sprite> title[2];
	std::unique_ptr<Sprite> tutorial[2];
	std::unique_ptr<Sprite> fadeSprite;
	XMFLOAT4 fadeColor = {1.0f,1.0f,1.0f,1.0f};

	bool isTutorial = false;
	unsigned int tutorialNum = 0;

	bool isFadeOut = false;
	bool isFadeIn = true;

	float titleObjectPosition = 0;
	float startObjectPosition = 0;

	bool isUp = false;
	int specifiedMoveTimer = 0;
	int maxSpecifiedMoveTimer = 15;
	float bounceAmount = 20.0f;
	float specifiedBouncePosUp = titleObjectPosition + bounceAmount;
	float specifiedBouncePosDown = titleObjectPosition - bounceAmount;
	float specifiedBounceStartPosUp = startObjectPosition + bounceAmount;
	float specifiedBounceStartPosDown = startObjectPosition - bounceAmount;
};


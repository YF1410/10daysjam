#include "AllLoadSprite.h"

void AllLoadSprite::AllLoadTexture()
{
	if (!Sprite::LoadTexture(1, L"Resources/sprite/circle.png"))
	{
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(2, L"Resources/sprite/triangle.png"))
	{
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(3, L"Resources/sprite/square.png"))
	{
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(4, L"Resources/sprite/cursor.png"))
	{
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(5, L"Resources/sprite/tile.png"))
	{
		assert(0);
		return;
	}

	/*if (!Sprite::LoadTexture(46, L"Resources/gameover.png")) {
		assert(0);
		return;
	}*/

}

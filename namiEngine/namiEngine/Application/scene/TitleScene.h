#pragma once
#include "SceneInterface.h"
class TitleScene : public SceneInterface
{
	void Initialize()override;
	void Finalize()override;
	void Update()override;
	void Draw()override;
};
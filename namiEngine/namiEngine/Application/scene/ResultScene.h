#pragma once
#include "SceneInterface.h"
class ResultScene : public SceneInterface
{
	void Initialize()override;
	void Finalize()override;
	void Update()override;
	void Draw()override;
};


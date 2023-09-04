#pragma once

#include <memory>

class SceneInterface
{
public:
	virtual ~SceneInterface() = default;
	virtual void Initialize() = 0;
	virtual void Finalize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
};


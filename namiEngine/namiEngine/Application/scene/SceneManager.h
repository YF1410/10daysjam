#pragma once
#include "SceneInterface.h"
class SceneManager
{
public:
	void Initialize(SceneInterface* scene);

	void Finalize();

	void Update();

	void Draw();

	void ToGameScene();

private:
	void changeScene(SceneInterface* scene);
private:
	SceneInterface* scene_;

public:
	static SceneManager* GetInstance();
private:
	static SceneManager instance;
private:
	SceneManager() = default;
	SceneManager(const SceneManager&) = delete;
	~SceneManager() = default;
	SceneManager& operator=(const SceneManager&) = delete;
};


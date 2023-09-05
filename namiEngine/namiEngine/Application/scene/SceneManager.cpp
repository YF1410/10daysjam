#include "SceneManager.h"
#include "GameScene.h"

SceneManager SceneManager::instance;

void SceneManager::Initialize(SceneInterface* scene)
{
	changeScene(scene);
	scene_->Initialize();
}

void SceneManager::Update()
{
	scene_->Update();
}

void SceneManager::Draw()
{
	scene_->Draw();
}

void SceneManager::ToGameScene()
{
	Initialize(new GameScene);
}

void SceneManager::changeScene(SceneInterface* scene)
{
	if (!scene_) {
		delete scene_;
	}
	scene_ = scene;
}

SceneManager* SceneManager::GetInstance()
{
	return &instance;
}

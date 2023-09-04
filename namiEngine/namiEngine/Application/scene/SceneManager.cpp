#include "SceneManager.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "GameClearScene.h"
#include "GameOverScene.h"
#include "TutorialScene.h"

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

void SceneManager::ToTitleScene()
{
	Initialize(new TitleScene);
}

void SceneManager::ToTutorialScene()
{
	Initialize(new TutorialScene);
}

void SceneManager::ToGameScene(StageManager::StageType stageType)
{
	StageManager::GetInstance()->SetStageType(stageType);
	Initialize(new GameScene);
}

void SceneManager::ToGameClearScene()
{
	Initialize(new GameClearScene);
}

void SceneManager::ToGameOverScene()
{
	Initialize(new GameOverScene);
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

#include "StageManager.h"

StageManager StageManager::instance;

StageManager* StageManager::GetInstance()
{
	return &instance;
}

StageManager::StageType StageManager::GetNextStageType() {
	StageType nextStage = static_cast<StageType>(nowStageType + 1);
	return nextStage;
}
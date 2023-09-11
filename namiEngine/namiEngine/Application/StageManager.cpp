#include "StageManager.h"

StageManager StageManager::instance;

StageManager* StageManager::GetInstance()
{
	return &instance;
}
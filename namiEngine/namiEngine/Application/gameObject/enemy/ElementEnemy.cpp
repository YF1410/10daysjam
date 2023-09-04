#include "ElementEnemy.h"

ElementEnemy::ElementEnemy()
{
	haveElement = true;
	defaultColor = { 1,0,0,1 };
	moveAmount = 0.4f;
	SetColor(defaultColor);
}

ElementEnemy::~ElementEnemy()
{
}

std::unique_ptr<ElementEnemy> ElementEnemy::Create(Player* player,Camera*camera) {
	std::unique_ptr<ElementEnemy> enemy = std::make_unique<ElementEnemy>();

	if (enemyModel == nullptr) {
		enemyModel = FbxLoader::GetInstance()->LoadModelFromFile("ZR");
	}

	enemy->SetModel(enemyModel.get());
	enemy->Initialize(player,camera);

	return enemy;
}
#include "TackleEnemy.h"

TackleEnemy::TackleEnemy()
{
	HP = 5;
	defaultColor = { 0,0,1,1 };
	SetColor(defaultColor);
	tackleRange.center = XMLoadFloat3(&position);
	tackleRange.radius = 25.0f;
}

TackleEnemy::~TackleEnemy()
{
}

std::unique_ptr<TackleEnemy> TackleEnemy::Create(Player* player, Camera* camera)
{
	std::unique_ptr<TackleEnemy> enemy = std::make_unique<TackleEnemy>();

	if (enemyModel == nullptr) {
		enemyModel = FbxLoader::GetInstance()->LoadModelFromFile("ZR");
	}

	enemy->SetModel(enemyModel.get());
	enemy->Initialize(player, camera);

	return enemy;
}

void TackleEnemy::Update()
{
	BaseEnemy::Update();

	if (isTackleRange && canMove) {
		tackleInterval--;
	}

	if (tackleInterval <= 0) {
		//tackleInterval = 60;
	}

	tackleRange.center = XMLoadFloat3(&position);
}

void TackleEnemy::Move()
{
	if (Collision::CheckSphere2Sphere(player->GetReceiveDamageCollision(), tackleRange)) {
		StopAnimation();
		tackleRange.radius = 50.0f;
		isTackleRange = true;
	}
	else {
		if (!isLoop) {
			LoopAnimation();
		}
		isTackleRange = false;
		moveAmount = 0.2f;
		tackleInterval = 60;
	}

	if (tackleInterval > 30 && canMove) {
		XMFLOAT3 pos = player->GetPosition();
		rad = atan2(pos.z - position.z, pos.x - position.x);
	}else if (tackleInterval <= 0) {
		Tackle(rad);
		if (tackleInterval <= -5) {
			canMove = false;
			tackleRange.radius = 25.0f;
			tackleInterval = 60;
		}
	}

	if (!canMove) {
		StopAnimation();
		moveCoolDown--;
		if (moveCoolDown <= 0) {
			canMove = true;
			moveCoolDown = 180;
		}
	}

	if (!isDamage && !isTackleRange && canMove) {
		savePos = position;
		moveX = (float)(cos(rad) * moveAmount + position.x);
		moveZ = (float)(sin(rad) * moveAmount + position.z);
	}



	if (isFirstMove) {
		savePos = position;
		moveX = (float)(cos(rad) * moveAmount + position.x);
		moveZ = (float)(sin(rad) * moveAmount + position.z);
		isFirstMove = false;
	}

	position = { moveX + shakeObjectPos[0], position.y + shakeObjectPos[1], moveZ + shakeObjectPos[2] };
	rotation = { 0,-XMConvertToDegrees(rad) + 90.0f,0 };
}

void TackleEnemy::Tackle(float rad) {
	moveAmount = 10.0f;
	moveX = (float)(cos(rad) * moveAmount + position.x);
	moveZ = (float)(sin(rad) * moveAmount + position.z);
}
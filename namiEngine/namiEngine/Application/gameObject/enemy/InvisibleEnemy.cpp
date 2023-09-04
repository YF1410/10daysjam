#include "InvisibleEnemy.h"
#include <random>

InvisibleEnemy::InvisibleEnemy()
{
	HP = 2;
	defaultColor = { 1,1,0,1 };
	SetColor(defaultColor);
}

InvisibleEnemy::~InvisibleEnemy()
{
}

std::unique_ptr<InvisibleEnemy> InvisibleEnemy::Create(Player* player, Camera* camera)
{
	std::unique_ptr<InvisibleEnemy> enemy = std::make_unique<InvisibleEnemy>();

	if (enemyModel == nullptr) {
		enemyModel = FbxLoader::GetInstance()->LoadModelFromFile("ZR");
	}

	enemy->SetModel(enemyModel.get());
	enemy->Initialize(player, camera);

	return enemy;
}

void InvisibleEnemy::Update()
{
	BaseEnemy::Update();
	if (!isInvisible) {
		shotInterval--;
		if (shotInterval <= 0 && !isShotCooldown) {
			shotInterval = 20;
			XMFLOAT3 playerPos = player->GetPosition();
			XMVECTOR move = XMVECTOR{ playerPos.x - position.x,playerPos.y - position.y,playerPos.z - position.z };
			move = XMVector3Normalize(move);
			move *= 0.7f;
			for (int i = -1; i < 2; i++) {
				XMVECTOR shotRad = XMVector3TransformNormal(move, XMMatrixRotationY(XMConvertToRadians(i * 20)));
				bullet.push_back(Bullet::Create(position, shotRad, player));
			}
			shotCount++;
			if (shotCount == 5) {
				isShotCooldown = true;
			}
		}

		if (isShotCooldown) {
			shotCooldown--;
			if (shotCooldown <= 0) {
				isShotCooldown = false;
				shotCooldown = 120;
				isInvisible = true;
				shotCount = 0;
				std::random_device seed_gen;
				std::mt19937_64 engine(seed_gen());
				std::uniform_real_distribution<float> posDist(-7.5f, 7.5f);
				randX = posDist(engine);
				randZ = posDist(engine);
			}
		}
	}
	else {
		invisibleTimer--;
		if (invisibleTimer <= 0) {
			isInvisible = false;
			invisibleTimer = 60;
		}
	}

	bullet.remove_if([](std::unique_ptr<Bullet>& bulletObj) {return !bulletObj->GetIsActive(); });

	for (std::unique_ptr<Bullet>& bulletObj : bullet) {
		bulletObj->Update(camera);
	}
}

void InvisibleEnemy::Draw(ID3D12GraphicsCommandList* cmdList)
{
	if (!isInvisible) {
		FbxObject3d::Draw(cmdList);
	}
	Object3d::PreDraw(cmdList);
	for (std::unique_ptr<Bullet>& bulletObj : bullet) {
		bulletObj->Draw();
	}
	colliderVisualizationObject->Draw();
	Object3d::PostDraw();
}

void InvisibleEnemy::Move()
{
	XMFLOAT3 pos = player->GetPosition();
	float rad = atan2(pos.z - position.z, pos.x - position.x);
	if (!isDamage && isInvisible) {
		savePos = position;
		moveX = randX * moveAmount + position.x;
		moveZ = randZ * moveAmount + position.z;
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
#include "BulletEnemy.h"

BulletEnemy::BulletEnemy()
{
	HP = 1.0f;
	defaultColor = { 0,1,0,1 };
	SetColor(defaultColor);
	shotRange.center = XMLoadFloat3(&position);
	shotRange.radius = 60.0f;
}

BulletEnemy::~BulletEnemy()
{
}

std::unique_ptr<BulletEnemy> BulletEnemy::Create(Player* player, Camera* camera) {
	std::unique_ptr<BulletEnemy> enemy = std::make_unique<BulletEnemy>();

	if (enemyModel == nullptr) {
		enemyModel = FbxLoader::GetInstance()->LoadModelFromFile("ZR");
	}

	enemy->SetModel(enemyModel.get());
	enemy->Initialize(player, camera);

	return enemy;
}

void BulletEnemy::Update() {
	BaseEnemy::Update();

	if (isShotRange) {
		shotInterval--;
	}

	if (shotInterval <= 0) {
		shotInterval = 120;
		XMFLOAT3 playerPos = player->GetPosition();
		XMVECTOR move = XMVECTOR{ playerPos.x - position.x,playerPos.y - position.y,playerPos.z - position.z };
		move = XMVector3Normalize(move);
		move *= 0.7f;

		for (int i = -1; i < 2; i++) {
			XMVECTOR shotRad = XMVector3TransformNormal(move, XMMatrixRotationY(XMConvertToRadians(i * 20)));
			bullet.push_back(Bullet::Create(position, shotRad, player));
		}
	}

	bullet.remove_if([](std::unique_ptr<Bullet>& bulletObj) {return !bulletObj->GetIsActive(); });

	for (std::unique_ptr<Bullet>& bulletObj : bullet) {
		bulletObj->Update(camera);
	}
	shotRange.center = XMLoadFloat3(&position);
}

void BulletEnemy::Draw(ID3D12GraphicsCommandList* cmdList) {
	FbxObject3d::Draw(cmdList);
	Object3d::PreDraw(cmdList);
	for (std::unique_ptr<Bullet>& bulletObj : bullet) {
		bulletObj->Draw();
	}
	colliderVisualizationObject->Draw();
	Object3d::PostDraw();
}

void BulletEnemy::Move()
{
	if (Collision::CheckSphere2Sphere(player->GetReceiveDamageCollision(), shotRange)) {
		isShotRange = true;
	}
	else {
		isShotRange = false;
		shotInterval = 90;
	}

	XMFLOAT3 pos = player->GetPosition();
	float rad = atan2(pos.z - position.z, pos.x - position.x);
	if (!isDamage && !isShotRange) {
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
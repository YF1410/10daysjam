#include "Bullet.h"

std::unique_ptr<Model> Bullet::bulletModel;
Sphere Bullet::skydomeCollider;

void Bullet::SetSkydomeCollier(Sphere skydomeCollider) {
	Bullet::skydomeCollider = skydomeCollider;
	Bullet::skydomeCollider.radius = 500.0f;
}

std::unique_ptr<Bullet> Bullet::Create(XMFLOAT3 startPos, XMVECTOR shotRad, Player* player)
{
	std::unique_ptr<Bullet> bullet = std::make_unique<Bullet>();

	bullet->Initialize(startPos, shotRad, player);
	bullet->SetScale(1.0f);
	bullet->SetColor({ 1,1,1,1 });

	return bullet;
}

bool Bullet::Initialize(XMFLOAT3 startPos, XMVECTOR shotRad, Player* player)
{
	if (bulletModel == nullptr) {
		bulletModel = Model::CreateFromObject("SphereCollider");
	}
	bulletObject = Object3d::Create(bulletModel.get());
	bulletObject->SetPosition(startPos);
	startPosition = startPos;
	bulletPosX = startPos.x;
	bulletPosZ = startPos.z;
	this->shotRad = shotRad;
	collision.radius = 1.0f;
	isActive = true;
	this->player = player;
	this->skydomeCollider = skydomeCollider;

	return true;
}

void Bullet::Update(Camera* camera)
{
	//XMStoreFloat2(&move, XMVector2Normalize(XMLoadFloat2(&move)));

	bulletPosX += shotRad.m128_f32[0];
	bulletPosZ += shotRad.m128_f32[2];

	collision.center = { bulletPosX,1.0f,bulletPosZ };
	bulletObject->SetPosition({ bulletPosX,1.0f,bulletPosZ });
	bulletObject->Update();

	if (Collision::CheckSphere2Sphere(collision, player->GetReceiveDamageCollision()) && !player->GetIsDash()) {
		isActive = false;
		if (player->GetHP() > 0 && !player->GetIsReceivedDamage()) {
			player->Damage(1);
			camera->SetShakeFlag(true, 6);
			player->SetIsReceivedDamage(true);
		}
	}
	
	if (!Collision::CheckSphere2Sphere(collision, skydomeCollider)) {
		isActive = false;
	}
}

void Bullet::Draw()
{
	bulletObject->Draw();
}
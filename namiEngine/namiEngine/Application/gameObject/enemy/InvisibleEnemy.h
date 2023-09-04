#pragma once
#include "BaseEnemy.h"
#include "Bullet.h"

class InvisibleEnemy : public BaseEnemy
{
public:
	static std::unique_ptr<InvisibleEnemy> Create(Player* player, Camera* camera);
public:
	//コンストラクタ
	InvisibleEnemy();
	//デストラクタ
	~InvisibleEnemy();

	void Update() override;

	void Draw(ID3D12GraphicsCommandList* cmdList) override;

	void Move() override;
private:
	int moveInterval = 60;
	int shotInterval = 30;
	int shotCooldown = 120;
	bool isShotCooldown = false;
	int shotCount = 0;
	int invisibleTimer = 60;
	float randX = 0;
	float randZ = 0;
	bool isInvisible = false;
	bool isFirstMove = true;
	std::list<std::unique_ptr<Bullet>> bullet;
};
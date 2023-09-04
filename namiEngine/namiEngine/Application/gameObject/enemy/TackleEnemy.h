#pragma once
#include "BaseEnemy.h"


class TackleEnemy : public BaseEnemy
{
public:
	static std::unique_ptr<TackleEnemy> Create(Player* player, Camera* camera);
public:
	//コンストラクタ
	TackleEnemy();
	//デストラクタ
	~TackleEnemy();

	void Update() override;

	void Move() override;

	void Tackle(float rad);
private:
	bool isFirstMove = true;
	Sphere tackleRange;
	bool isTackleRange;
	int tackleInterval;
	float rad;
	bool canMove;
	int moveCoolDown = 20;
};


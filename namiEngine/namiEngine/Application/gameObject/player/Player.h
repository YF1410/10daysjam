#pragma once
#include "Object3d.h"
#include "FbxObject3d.h"
#include "CollisionPrimitive.h"
#include "input.h"
#include "Vector3.h"
#include "BaseEnemy.h"

class BaseEnemy;
class GameScene;

class Player :
	public FbxObject3d {
public:
	/// <summary>
	/// 3Dオブジェクト生成
	/// </summary>
	/// <returns></returns>
	static std::unique_ptr<Player> Create(std::list<std::unique_ptr<BaseEnemy>>* enemy);

public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	void Initialize(std::list<std::unique_ptr<BaseEnemy>>* enemy);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void CollisionUpdate();

	/// <summary>
	/// コンボ処理
	/// </summary>
	void Combo();

	/// <summary>
	/// モーション変更
	/// </summary>
	void ChangeMotion();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// 攻撃処理
	/// </summary>
	void Attack();

	/// <summary>
	/// 移動処理
	/// </summary>
	void Move(Vector3 vec);

	/// <summary>
	/// ダメージ処理
	/// </summary>
	/// <param name="damageQuantity"></param>
	void Damage(int damageQuantity) { HP -= damageQuantity; }

	//ゲッター
	float GetAttackPowor() { return attackPowor; }
	XMFLOAT3 GetPlayerPos() { return playerPos; }
	Sphere GetReceiveDamageCollision() { return receiveDamageCollision; }
	Sphere GetInflictDamageCollision() { return inflictDamageCollision; }
	bool GetIsAttack() { return isAttack; }
	bool GetIsHardAttack() { return isHardAttack; }
	bool GetIsDash() { return isDash; }
	bool GetIsReceivedDamage() { return isReceivedDamage; }
	bool GetIsActive() { return isActive; }
	int GetHP() { return HP; }
	int GetDashCount() { return dashCount; }
	int GetAttackCount() { return attackCount; }
	bool GetIsHaveElement() { return isHaveElement; }
	float GetMoveAmount() { return moveAmount; }

	//セッター
	void SetIsReceivedDamage(bool isReceivedDamage) { this->isReceivedDamage = isReceivedDamage; }
	void SetIsHaveElement(bool isHaveElement) { this->isHaveElement = isHaveElement; }
	void SetIsNowCameraShake(bool isNowCameraShake) { this->isNowCameraShake = isNowCameraShake; }
	void SetPlayerPos(XMFLOAT3 playerPos) { this->playerPos = playerPos; }
	void SetDefColor(XMFLOAT4 color) { defColor = color; }
private:
	std::list<std::unique_ptr<BaseEnemy>>* enemy;	//エネミー配列

	Input* input = nullptr;	//入力
	XMFLOAT3 playerPos = { 0.0f,0.0f,0.0f };	//移動時に使用
	XMFLOAT3 savePos = { 0.0f,0.0f,0.0f };		//ポジションの保存用
	int HP = 3;
	const float defMoveAmount = 1.0f;			//基本の移動量
	const float dashMoveAmount = 1.2f;			//ダッシュ中の移動量
	int dashTimer = 30;							//ダッシュの持続時間
	int damageTimer = 60;						//ダメージくらった時の無敵時間

	int dashCount = 0;							//チュートリアルでダッシュ回数を数えるために使用
	int attackCount = 0;						//チュートリアルで攻撃回数を数えるために使用

	XMFLOAT4 defColor = { 1,1,1,1 };			//基本の色

	float attackPowor = 1;						//攻撃した時に与えるダメージ
	int attackTimer = 15;						//攻撃の持続時間
	bool isNowCombo = false;					//コンボ中かどうか
	int comboCount = 0;							//今何コンボ目かのカウント
	int comboTimer = 60;						//コンボが切れるまでの時間
	bool isRigor = false;						//今硬直している時間かどうか
	int rigorTimer = 15;						//硬直が切れるまでの時間
	bool isActive = true;						//生きているかどうか
	bool isHaveElement = false;					//属性を持っているかどうか
	bool isDash = false;						//ダッシュしているかどうか
	bool isAttack = false;					//弱攻撃しているかどうか
	bool isHardAttack = false;					//強攻撃しているかどうか
	bool isReceivedDamage = false;				//攻撃を受けているかどうか
	bool isNowCameraShake = false;				//カメラシェイクをしているかどうか
	bool isMove = false;						//動いているかどうか
	bool isIdle = false;						//待機中かどうか
	Sphere receiveDamageCollision;				//ダメージを受ける当たり判定
	Sphere inflictDamageCollision;				//ダメージを与えられる当たり判定
	float moveAmount;							//移動量
	std::unique_ptr<Model> receiveDamageColliderVisualizationModel;			//敵が触れるとダメージを受ける判定を可視化する用のモデル
	std::unique_ptr<Object3d> receiveDamageColliderVisualizationObject;		//敵が触れるとダメージを受ける判定を可視化する用のオブジェクト
	std::unique_ptr<Model> inflictDamageColliderVisualizationModel;			//攻撃中に範囲内に敵が入るとダメージを与えられる判定を可視化する用のモデル
	std::unique_ptr<Object3d> inflictDamageColliderVisualizationObject;		//攻撃中に範囲内に敵が入るとダメージを与えられる判定を可視化する用のオブジェクト
	std::unique_ptr<FbxModel> attackModel;		//攻撃時のモデル
	std::unique_ptr<FbxModel> moveModel;		//移動時のモデル
	std::unique_ptr<FbxModel> rollModel;		//ダッシュ時のモデル
	std::unique_ptr<FbxModel> idleModel;		//操作していない時のモデル
	//Mediatorパターン

	//止まっている時に行動ゲージチャージ
	//動いたり攻撃で減っていく
};
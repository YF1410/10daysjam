#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <string>

#include "DirectXCommon.h"
#include "Model.h"
#include "Camera.h"
#include "LightGroup.h"
#include "CollisionInfo.h"

class BaseCollider;

/// <summary>
/// 3Dオブジェクト
/// </summary>
class Object3d {
protected: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // サブクラス

	// パイプラインセット
	struct PipelineSet 	{
		// ルートシグネチャ
		ComPtr<ID3D12RootSignature> rootsignature;
		// パイプラインステートオブジェクト
		ComPtr<ID3D12PipelineState> pipelinestate;
	};

	// 定数バッファ用データ構造体B0
	struct ConstBufferDataB0 	{
		XMMATRIX viewproj;    // ビュープロジェクション行列
		XMMATRIX world; // ワールド行列
		XMFLOAT3 cameraPos; // カメラ座標（ワールド座標）
		float pad;
		XMFLOAT4 color;
	};

public: // 静的メンバ関数
	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	static void StaticInitialize(Camera* camera = nullptr);

	/// <summary>
	/// 静的解放
	/// </summary>
	static void StaticFinalize();

	/// <summary>
	/// グラフィックパイプラインの生成
	/// </summary>
	static void CreateGraphicsPipeline();

	/// <summary>
	/// カメラのセット
	/// </summary>
	/// <param name="camera">カメラ</param>
	static void SetCamera(Camera* camera) {
		Object3d::cameraObject = camera;
	}

	/// <summary>
	/// ライトグループのセット
	/// </summary>
	/// <param name="lGroup">ライトグループ</param>
	static void SetLightGroup(LightGroup* lGroup) {
		Object3d::lightGroup = lGroup;
	}

	/// <summary>
	/// 描画前処理
	/// </summary>
	/// <param name="cmdList">描画コマンドリスト</param>
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// 描画後処理
	/// </summary>
	static void PostDraw();

	/// <summary>
	/// 3Dオブジェクト生成
	/// </summary>
	/// <returns></returns>
	static std::unique_ptr<Object3d> Create(Model* model = nullptr);

private: // 静的メンバ変数
	// デバイス
	static ComPtr<ID3D12Device> device;
	// コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	// パイプライン
	static PipelineSet pipelineSet;
	// カメラ
	static Camera* cameraObject;
	// ライト
	static LightGroup* lightGroup;

public: // メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Object3d() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Object3d();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	virtual bool Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// 座標の取得
	/// </summary>
	/// <returns>座標</returns>
	const XMFLOAT3& GetPosition() { return objPosition; }

	/// <summary>
	/// 回転の取得
	/// </summary>
	/// <returns>回転</returns>
	const XMFLOAT3& GetRotation() { return objRotation; }

	/// <summary>
	/// ワールド行列の取得
	/// </summary>
	/// <returns>ワールド行列</returns>
	const XMMATRIX& GetMatWorld() { return matWorld; }
	
	void SetColor(XMFLOAT4 color) { objColor = color; }

	/// <summary>
	/// 座標の設定
	/// </summary>
	/// <param name="position">座標</param>
	void SetPosition(XMFLOAT3 position) { this->objPosition = position; }

	/// <summary>
	/// 回転の設定
	/// </summary>
	/// <param name="rotation">回転量</param>
	void SetRotation(XMFLOAT3 rotation) { this->objRotation = rotation; }

	/// <summary>
	/// スケールの設定
	/// </summary>
	/// <param name="scale">スケール</param>
	void SetScale(XMFLOAT3 scale) { this->objScale = scale; }

	void SetScale(float scale) { this->objScale = { scale ,scale,scale }; }

	/// <summary>
	/// モデルのセット
	/// </summary>
	/// <param name="model">モデル</param>
	void SetModel(Model* model) { this->objModel = model; }

	/// <summary>
	/// ビルボードフラグのセット
	/// </summary>
	/// <param name="isSetBillboard">ビルボードか</param>
	void SetBillboard(bool isSetBillboard) { this->isBillboard = isSetBillboard; }

	/// <summary>
	/// コライダーのセット
	/// </summary>
	/// <param name="collider">コライダー</param>
	void SetCollider(BaseCollider* collider);

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	/// <param name="info">衝突情報</param>
	virtual void OnCollision(const CollisionInfo& info) {}

	/// <summary>
	/// ワールド座標を取得
	/// </summary>
	/// <returns></returns>
	XMFLOAT3 GetWorldPosition();

	/// <summary>
	/// モデルを取得
	/// </summary>
	/// <returns>モデル</returns>
	inline Model* GetModel() { return objModel; }

protected: // メンバ変数

	const char* name = nullptr;

	ComPtr<ID3D12Resource> constBuffB0; // 定数バッファ
	// 色
	XMFLOAT4 objColor = { 1,1,1,1 };
	// ローカルスケール
	XMFLOAT3 objScale = { 1,1,1 };
	// X,Y,Z軸回りのローカル回転角
	XMFLOAT3 objRotation = { 0,0,0 };
	// ローカル座標
	XMFLOAT3 objPosition = { 0,0,0 };
	// ローカルワールド変換行列
	XMMATRIX matWorld;
	// 親オブジェクト
	Object3d* parent = nullptr;
	// モデル
	Model* objModel = nullptr;
	// ビルボード
	bool isBillboard = false;
	// コライダー
	BaseCollider* collider = nullptr;
};

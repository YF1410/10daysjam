#pragma once

#include "FbxModel.h"
#include "Camera.h"
#include "FbxLoader.h"
#include "DirectXCommon.h"
#include "LightGroup.h"

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <string>

class FbxObject3d {
protected: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public://定数
//ボーンの最大数
	static const int MAX_BONES = 32;

public: // サブクラス
	// 定数バッファ用データ構造体（座標変換行列用）
	struct ConstBufferDataTransform {
		XMMATRIX viewproj;    // ビュープロジェクション行列
		XMMATRIX world; // ワールド行列
		XMFLOAT3 cameraPos; // カメラ座標（ワールド座標）
		float pad;
		XMFLOAT4 color;
	};

	//定数バッファ用データ構造体(スキニング)
	struct ConstBufferDataSkin {
		XMMATRIX bones[MAX_BONES];
	};

public: // 静的メンバ関数
	/// <summary>
	/// グラフィックパイプラインの生成
	/// </summary>
	static void CreateGraphicsPipeline();
	static void StaticFinalize();
	// setter
	static void SetCamera(Camera* cameraObject) { FbxObject3d::cameraObject = cameraObject; }

private: // 静的メンバ変数
	// デバイス
	static ComPtr<ID3D12Device> device;
	// カメラ
	static Camera* cameraObject;
	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootsignature;
	// パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelinestate;

public: // メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList);

	void SetModel(FbxModel* fbxmodel) { this->fbxmodel = fbxmodel; }

	/// <summary>
	/// アニメーション開始
	/// </summary>
	void PlayAnimation(bool isResetTime);

	/// <summary>
	/// ループアニメーション開始
	/// </summary>
	void LoopAnimation();

	/// <summary>
	/// アニメーション停止
	/// </summary>
	void StopAnimation();

	//	座標の取得
	inline const XMFLOAT3& GetPosition() { return position; }
	// X,Y,Z軸回りの取得
	inline const XMFLOAT3& GetRotation() { return rotation; }
	// スケールの取得
	inline const XMFLOAT3& GetScale() { return scale; }
	// 座標の設定
	inline void SetPosition(XMFLOAT3 position) { this->position = position; }
	// X,Y,Z軸回りの設定
	inline void SetRotation(XMFLOAT3 rotation) { this->rotation = rotation; }
	// スケールの設定
	inline void SetScale(XMFLOAT3 scale) { this->scale = scale; }

	void SetColor(XMFLOAT4 color) { this->color = color; }

	bool GetIsPlay() { return isPlay; }

	void ResetAnimationTime();

protected: // メンバ変数
	// 定数バッファ（座標変換行列用）
	ComPtr<ID3D12Resource> constBuffTransform;
	//定数バッファ(スキン)
	ComPtr<ID3D12Resource> constBuffSkin;
	//ライトグループ
	std::unique_ptr<LightGroup> lightGroup;
	// ローカルスケール
	XMFLOAT3 scale = { 1,1,1 };
	// X,Y,Z軸回りのローカル回転角
	XMFLOAT3 rotation = { 0,0,0 };
	// ローカル座標
	XMFLOAT3 position = { 0,0,0 };
	// ローカルワールド変換行列
	XMMATRIX matWorld;
	//色
	XMFLOAT4 color = { 1,1,1,1 };
	// モデル
	FbxModel* fbxmodel = nullptr;
	//1フレームの時間
	FbxTime frameTime;
	//アニメーション開始時間
	FbxTime startTime;
	//アニメーション終了時間
	FbxTime endTime;
	//現在時間(アニメーション
	FbxTime currentTime;
	//中断した時間(アニメーション
	FbxTime saveTime;
	//アニメーション再生中
	bool isPlay = false;
	//アニメーションループ再生中
	bool isLoop = false;

	XMFLOAT3 circleShadowDir = { 0,-1,0 };
	XMFLOAT3 circleShadowAtten = { 0.5f,0.6f,0.0f };
	XMFLOAT2 circleShadowFactorAngle = { 0.0f,0.5f };
};
#pragma once

#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <d3dx12.h>
#include <cstdlib>
#include <chrono>

#include "WinApp.h"
#include "FixFPS.h"

class DirectXCommon {
private: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	
public:
	static DirectXCommon* GetInstance();


public: // メンバ関数
	/*~DirectXCommon();*/
	// 初期化
	void Initialize(HWND hwnd);
	// 描画前処理
	void PreDraw();
	/// 描画後処理
	void PostDraw();
	// レンダーターゲットのクリア
	void ClearRenderTarget();
	// 深度バッファのクリア
	void ClearDepthBuffer();
	// デバイスの取得
	ComPtr<ID3D12Device> GetDevice() { return device; }
	// 描画コマンドリストの取得
	ID3D12GraphicsCommandList* GetCommandList() { return commandList.Get(); }

private: // メンバ変数
	// ウィンドウズアプリケーション管理
	HWND hwnd;

	// Direct3D関連
	ComPtr<IDXGIFactory6> dxgiFactory;
	ComPtr<ID3D12Device> device;
	ComPtr<ID3D12GraphicsCommandList> commandList;
	ComPtr<ID3D12CommandAllocator> commandAllocator;
	ComPtr<ID3D12CommandQueue> commandQueue;
	ComPtr<IDXGISwapChain4> swapchain;
	std::vector<ComPtr<ID3D12Resource>> backBuffers;
	ComPtr<ID3D12Resource> depthBuffer;
	ComPtr<ID3D12DescriptorHeap> rtvHeaps;
	ComPtr<ID3D12DescriptorHeap> dsvHeap;
	ComPtr<ID3D12Fence> fence;
	UINT64 fenceVal = 0;

	ComPtr<ID3D12DescriptorHeap> imguiHeap;
	float deltaTime = 0.0f;
	float frameRate = 0.0f;
	float commandWaitTime = 0.0f;
	std::chrono::steady_clock::time_point lastUpdate;

	std::unique_ptr<FixFPS> fixFPS;

private: // メンバ関数
	/// <summary>
	/// DXGIデバイス初期化
	/// </summary>
	/// <returns>成否</returns>
	bool InitializeDXGIDevice();

	/// <summary>
	/// スワップチェーンの生成
	/// </summary>
	/// <returns>成否</returns>
	bool CreateSwapChain();

	/// <summary>
	/// コマンド関連初期化
	/// </summary>
	/// <returns>成否</returns>
	bool InitializeCommand();

	/// <summary>
	/// レンダーターゲット生成
	/// </summary>
	/// <returns>成否</returns>
	bool CreateFinalRenderTargets();

	/// <summary>
	/// 深度バッファ生成
	/// </summary>
	/// <returns>成否</returns>
	bool CreateDepthBuffer();

	/// <summary>
	/// フェンス生成
	/// </summary>
	/// <returns>成否</returns>
	bool CreateFence();

	/// <summary>
	/// imgui初期化
	/// </summary>
	/// <returns>成否</returns>
	bool InitImgui();

private:
	static DirectXCommon instance;

private:
	DirectXCommon() = default;
	DirectXCommon(const DirectXCommon&) = delete;
	~DirectXCommon() = default;
	DirectXCommon& operator=(const DirectXCommon&) = delete;
};
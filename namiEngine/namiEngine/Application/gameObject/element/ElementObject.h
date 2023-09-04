#pragma once
#include "Object3d.h"
#include "FbxObject3d.h"
#include "CollisionPrimitive.h"

class ElementObject : public FbxObject3d
{
protected: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	static std::unique_ptr<ElementObject> Create(FbxModel* fbxmodel,XMFLOAT3 pos);
public:
	ElementObject();
	~ElementObject();
	void Initialize();
	void Update();
	void Draw(ID3D12GraphicsCommandList* cmdList);
	bool GetIsActive() { return isActive; }
	Sphere GetCollision() { return collision; }
	void SetIsActive(bool isActive) { this->isActive = isActive; }
private:
	float elementPos[3] = { 10.0f,0,20.0f };
	bool isActive = true;
	Sphere collision;
	std::unique_ptr<Model> colliderVisualizationModel;
	std::unique_ptr<Object3d> colliderVisualizationObject;
};


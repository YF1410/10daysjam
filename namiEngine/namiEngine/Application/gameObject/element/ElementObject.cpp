#include "ElementObject.h"

ElementObject::ElementObject() {

}

ElementObject::~ElementObject() {

}

std::unique_ptr<ElementObject> ElementObject::Create(FbxModel* fbxmodel, XMFLOAT3 pos) {
	std::unique_ptr<ElementObject> element = std::make_unique<ElementObject>();

	element->Initialize();
	element->SetModel(fbxmodel);
	XMFLOAT3 elementPos = { pos.x,0,pos.z };
	element->SetPosition(elementPos);
	element->SetRotation({ 0,90.0f,0, });
	element->SetColor({ 0,0,1,1 });
	element->collision.center = XMLoadFloat3(&elementPos);
	element->collision.radius = 1.0f;
	element->LoopAnimation();

	return element;
}

void ElementObject::Initialize() {
	FbxObject3d::Initialize();
	colliderVisualizationModel = Model::CreateFromObject("SphereCollider");
	colliderVisualizationObject = Object3d::Create(colliderVisualizationModel.get());
	colliderVisualizationObject->SetPosition(position);
	colliderVisualizationObject->SetScale(collision.radius);
	colliderVisualizationObject->SetColor({ 0,0,1,0.3f });
}

void ElementObject::Update() {
	collision.center = XMLoadFloat3(&position);
	colliderVisualizationObject->SetPosition(position);
	colliderVisualizationObject->SetScale(collision.radius);
	colliderVisualizationObject->Update();
	FbxObject3d::Update();
}

void ElementObject::Draw(ID3D12GraphicsCommandList* cmdList) {
	FbxObject3d::Draw(cmdList);
	Object3d::PreDraw(cmdList);
	colliderVisualizationObject->Draw();
	Object3d::PostDraw();
}
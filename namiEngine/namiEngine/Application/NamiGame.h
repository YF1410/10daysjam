#pragma once
#include "Framework.h"

#include "Object3d.h"
#include "FbxObject3d.h"
#include "ParticleManager.h"
#include "SceneManager.h"
#include "PostEffect.h"

class NamiGame : public Framework {
public:
	//初期化
	void Initialize() override;
	//終了
	void Finalize() override;
	//毎フレーム更新
	void Update() override;
	//描画
	void Draw() override;
private:
	SceneManager* scene_;
	std::unique_ptr<PostEffect> postEffect;
};
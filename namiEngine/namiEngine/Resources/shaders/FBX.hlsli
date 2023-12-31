//ボーンの最大数
static const int MAX_BONES = 32;

cbuffer cbuff0 : register(b0) {
	matrix viewproj; // ビュープロジェクション行列
	matrix world; // ワールド行列
	float3 cameraPos; // カメラ座標（ワールド座標）
	float1 pad;
	float4 color;
};
// 平行光源の数
static const int DIRLIGHT_NUM = 3;

struct DirLight {
	float3 lightv;    // ライトへの方向の単位ベクトル
	float3 lightColor;    // ライトの色(RGB)
	uint lightActive;
};

// 点光源の数
static const int POINTLIGHT_NUM = 3;

struct PointLight {
	float3 lightPos;    // ライト座標
	float3 lightColor;  // ライトの色(RGB)
	float3 lightatten;	// ライト距離減衰係数
	uint lightActive;
};

// スポットライトの数
static const int SPOTLIGHT_NUM = 3;

struct SpotLight {
	float3 lightv;		// ライトの光線方向の逆ベクトル（単位ベクトル）
	float3 lightPos;    // ライト座標
	float3 lightColor;  // ライトの色(RGB)
	float3 lightatten;	// ライト距離減衰係数
	float2 lightfactoranglecos; // ライト減衰角度のコサイン
	uint lightActive;
};

// 丸影の数
static const int CIRCLESHADOW_NUM = 1;

struct CircleShadow {
	float3 dir;		// 投影方向の逆ベクトル（単位ベクトル）
	float3 casterPos;    // キャスター座標
	float  distanceCasterLight;	// キャスターとライトの距離
	float3 lightAtten;	// 距離減衰係数
	float2 factorAngleCos; // 減衰角度のコサイン
	uint lightActive;
};

cbuffer cbuff3 : register(b3) {
	float3 ambientColor;
	DirLight dirLights[DIRLIGHT_NUM];
	PointLight pointLights[POINTLIGHT_NUM];
	SpotLight spotLights[SPOTLIGHT_NUM];
	CircleShadow circleShadows[CIRCLESHADOW_NUM];
}

cbuffer skinning : register(b2) {
	matrix matSkinning[MAX_BONES];
};

//バーテックスバッファーの入力
struct VSInput {
	float4 pos	: POSITION;				//位置
	float3 normal : NORMAL;				//頂点法線
	float2 uv	: TEXCOORD;				//テクスチャー座標
	uint4 boneIndices : BONEINDICES;	//ボーンの番号
	float4 boneWeights :BONEWEIGHTS;		//ボーンのスキンウェイト
};

// 頂点シェーダーからピクセルシェーダーへのやり取りに使用する構造体
struct VSOutput {
	float4 svpos : SV_POSITION; // システム用頂点座標
	float4 worldpos : POSITION; // ワールド座標
	float3 normal :NORMAL; // 法線
	float2 uv  :TEXCOORD; // uv値
};

#include "PostEffectTest.hlsli"

Texture2D<float4> tex0 : register(t0);  // 0番スロットに設定されたテクスチャ
Texture2D<float4> tex1 : register(t1);  // 1番スロットに設定されたテクスチャ
SamplerState smp : register(s0);       // 0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
	float4 tex0color = tex0.Sample(smp, input.uv);
	float4 tex1color = tex1.Sample(smp, input.uv);

	float _ShiftWidth = 0.005f;
	float _ShiftNum = 3;

	float4 col = { 0,0,0,0 };
	float num = 0;

	for (float py = -_ShiftNum / 2; py <= _ShiftNum / 2; py++) {
		for (float px = -_ShiftNum / 2; px <= _ShiftNum / 2; px++) {
			col += tex0.Sample(smp, input.uv + float2(px,py) * _ShiftWidth);
			num++;
		}
	}

	float4 color = col.rgba/num;
	if (fmod(input.uv.y, 0.1f) < 0.05f) {
		color = tex1color;
	}

	return float4(color.rgb, 1);
}
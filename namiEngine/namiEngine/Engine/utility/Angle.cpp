#include "Angle.h"

void Angle::Initialize() {
}

void Angle::Update() {
	angle1 = endPos1 - startPos1;
	angle2 = endPos2 - startPos2;

	resultRadians = static_cast<float>(atan2(angle1, angle2));
}

float Angle::GetRadians() {
	return resultRadians;
}

float Angle::PosForRadians(float startPos1, float startPos2, float endPos1, float endPos2) {
	angle1 = endPos1 - startPos1;
	angle2 = endPos2 - startPos2;

	resultRadians = static_cast<float>(atan2(angle1, angle2));

	return resultRadians;
}

float Angle::lR(float startPos1, float startPos2, float endPos1, float endPos2) {
	float px = endPos1 - startPos1;
	float py = endPos2 - startPos2;

	float l = static_cast<float>(sqrt(px * px + py * py));
	return l;
}
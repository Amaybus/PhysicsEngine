#pragma once

#include "PhysicsObject.h"

class Box : public PhysicsObject
{
	float mWidth;
	float mHeight;

	Vec2 mMax;
	Vec2 mMin;


public:
	Box(Vec2 pos, float w, float h, float mass);

	void Update(float delta) override;
	void Draw(LineRenderer* lines) override;


	const float GetWidth() { return mWidth; }
	const float GetHeight() { return mHeight; }
	Vec2 GetMax() { return mMax; }
	Vec2 GetMin() { return mMin; }
};


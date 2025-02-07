#pragma once

#include "PhysicsObject.h"

class Box : public PhysicsObject
{
	float mWidth;
	float mHeight;

public:
	Vec2 mYMax;
	Vec2 mYMin;
	
	Vec2 mXMax;
	Vec2 mXMin;


	Box(Vec2 pos, float w, float h, float mass);

	void Draw(LineRenderer* lines) override;


	const float GetWidth() { return mWidth; }
	const float GetHeight() { return mHeight; }
};


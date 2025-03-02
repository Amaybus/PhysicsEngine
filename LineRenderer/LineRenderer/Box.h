#pragma once
#include "Polygon.h"
#include "PhysicsObject.h"

class Box : public Polygon
{
	float mWidth;
	float mHeight;

public:
	float GetXMin() const { return mPos.x - 0.5 * mWidth; }
	float GetXMax() const { return mPos.x + 0.5 * mWidth; }
	float GetYMin() const { return mPos.y - 0.5 * mHeight; }
	float GetYMax() const { return mPos.y + 0.5 * mHeight; }

	Box(Vec2 pos, float w, float h, float mass);
	Box(Vec2 pos, float w, float h, float mass, float elasticity);

	void Draw(LineRenderer* lines) override;

	float GetWidth() const  { return mWidth; }
	float GetHeight() const { return mHeight; }
	int GetType() const override { return 2; }
};


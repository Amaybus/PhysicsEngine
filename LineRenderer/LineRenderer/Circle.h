#pragma once
#include "Colour.h"
#include "PhysicsObject.h"

class LineRenderer;
class PhysicsObject;

class Circle : public PhysicsObject
{
	const float mRadius;

	Vec2 mLocalX;
	Vec2 mLocalY;

public:
	Circle(Vec2 pos, float radius, float mass);

	float GetRadius() const { return mRadius; }
	int GetType() const override { return 0; }

	void Draw(LineRenderer* lines) override;
};


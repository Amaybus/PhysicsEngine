#pragma once
#include "Colour.h"
#include "PhysicsObject.h"

class LineRenderer;
class PhysicsObject;

class Circle : public PhysicsObject
{
	const float mRadius;

public:
	Circle(Vec2 pos, float radius, float mass);

	void Draw(LineRenderer* lines) override;

	float GetRadius() const { return mRadius; }
	int GetType() const override { return 0; }
};


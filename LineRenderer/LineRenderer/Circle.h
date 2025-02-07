#pragma once
#include "Colour.h"
#include "PhysicsObject.h"

class LineRenderer;
class PhysicsObject;

class Circle : public PhysicsObject
{
	const float mRadius;

public:
	Circle(Vec2 pos, float mass, float radius);

	float GetRadius() const { return mRadius; }

	void Draw(LineRenderer* lines) override;
};


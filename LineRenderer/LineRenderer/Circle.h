#pragma once
#include "Colour.h"
#include "PhysicsObject.h"

class LineRenderer;
class PhysicsObject;

class Circle : public PhysicsObject
{
	const float mRadius;
	Colour mColour;

public:
	Circle(Vec2 pos, float mass, float radius);

	float GetRadius() const { return mRadius; }
	void SetColour(Colour colour) { mColour = colour; }

	void Update(float delta) override;
	void Draw(LineRenderer* lines) override;
};


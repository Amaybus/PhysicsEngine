#pragma once
#include "PhysicsObject.h"
#include <vector>

class Polygon : public PhysicsObject
{
	const int mVertCount;
	

public:
	std::vector<Vec2> mVertices;

	Polygon(Vec2 pos, int numOfVerts, float mass);

	int GetVertexCount() const { return mVertCount; }

	void Update(float delta) override;
	void Draw(LineRenderer* lines) override;
};


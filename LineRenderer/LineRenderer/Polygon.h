#pragma once
#include "PhysicsObject.h"
#include <vector>

class Polygon : public PhysicsObject
{
	const int mVertCount;
	
	// Rotation axis
	Vec2 mLocalX;
	Vec2 mLocalY;

public:
	std::vector<Vec2> mVertices;
	std::vector<Vec2> mNormals;
	std::vector<Vec2> mEdgeCentre;

	Polygon(Vec2 pos, int numOfVerts, float mass);

	int GetVertexCount() const { return mVertCount; }
	int GetType() const override { return 3; }

	void Draw(LineRenderer* lines) override;
};


#pragma once
#include "PhysicsObject.h"
#include <vector>

class Polygon : public PhysicsObject
{
	const int mVertCount;
	

public:
	std::vector<Vec2> mVertices;
	std::vector<Vec2> mNormals;
	std::vector<Vec2> mEdgeCentre;

	Polygon(Vec2 pos, int numOfVerts, float mass);

	int GetVertexCount() const { return mVertCount; }
	int GetType() const override { return 3; }

	//void Update(float delta) override;
	void Draw(LineRenderer* lines) override;
};


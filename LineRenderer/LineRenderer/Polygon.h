#pragma once
#include "PhysicsObject.h"
#include <vector>
#include <memory>

class Polygon : public PhysicsObject
{
	const int mVertCount;
	float mPadding;

public:
	std::vector<Vec2> mVertices;
	std::vector<Vec2> mNormals;
	std::vector<Vec2> mEdgeCentres;

	Polygon(Vec2 pos, int numOfVerts, float mass);
	Polygon(Vec2 pos, int numOfVerts, float mass, float elasticity);
	Polygon(const Polygon& other) = delete;
	Polygon& operator=(const Polygon& other) = delete;
	// Box constructor
	Polygon(Vec2 pos, float mass);

	~Polygon();

	std::vector<Vec2> GetVertices() { return mVertices; }
	std::vector<Vec2> GetNormals();
	std::vector<Vec2> GetEdgeCentres();
	int GetVertexCount() const { return mVertCount; }
	int GetType() const override { return 3; }
	void Draw(LineRenderer* lines) override;
	float GetPadding() const { return mPadding; }
};


#include "PhysicsEngine.h"
#include "CollisionFunctions.h"
#include "CollisionInfo.h"
#include "LineRenderer.h"


PhysicsEngine::PhysicsEngine()
{
}

void PhysicsEngine::Initialise()
{
	appInfo.appName = "Physics Engine";

	// Draw Circle on mouse pos
	mCircles.push_back(new Circle(cursorPos, 1, 0.5f));

	// Create a circle to add to test against
	mCircles.push_back(new Circle(Vec2{ 1,10 }, 10, 1));
	//mCircles.push_back(new Circle(Vec2{ 10,1 }, 1, 1));
	//mCircles.push_back(new Circle(Vec2{ 4,7 }, 16, 4));

	mBoxes.push_back(new Box(Vec2(1, 1), 1, 1, 1));


	mPlanes[0] = new Plane(Vec2(0, 1), 1, 5);
}

void PhysicsEngine::Update(float delta)
{
	// THIS AINT RIGHT LMAO
	lines->DrawLineSegment(Vec2(mBoxes[0]->GetMax().x - mBoxes[0]->GetWidth(), mBoxes[0]->GetMin().y - mBoxes[0]->GetHeight()), mBoxes[0]->GetMax());

	mCircles[0]->SetPos(cursorPos);
	mCircles[1]->SetPos(Vec2(1, mCircles[1]->GetPos().y - 0.1f));
	for (Circle* circ : mCircles)
	{
		circ->Draw(lines);
	}

	mPlanes[0]->Draw(lines);

	CollisionInfo info = PlaneToCircleCollision(mPlanes[0], mCircles[1]);
	if (info.bIsOverlapping) { mCircles[1]->SetColour(Colour::RED); info.Resolve(); }
	else mCircles[1]->SetColour(Colour::GREEN);


	for (int i = 1; i < mCircles.size() - 1; i++)
	{
		for (int j = i + 1; j < mCircles.size(); j++)
		{
			//CollisionInfo info = CircleToCircleCollision(mCircles[i], mCircles[j]);
			////lines->DrawText(std::to_string(info.overlapAmount), Vec2{ cursorPos.x, cursorPos.y + 1 }, 1);
			////if (info.bIsOverlapping) { mCircles[0]->SetColour(Colour::RED); info.Resolve(); }
			//if (info.bIsOverlapping) { mCircles[j]->SetColour(Colour::RED); info.Resolve(); }
			//else mCircles[j]->SetColour(Colour::GREEN);
		}
	}
}


void PhysicsEngine::OnLeftClick()
{
}

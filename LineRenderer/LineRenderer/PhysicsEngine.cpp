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
	mCircles.push_back(new Circle(cursorPos, 1, 0.62f));

	// Create a circle to add to test against
	mCircles.push_back(new Circle(Vec2{ 1.5,1.5 }, 10, 0.75f));
}

void PhysicsEngine::Update(float delta)
{
	mCircles[0]->SetPos(cursorPos);
	for(Circle* circ : mCircles)
	{
		circ->Draw(lines);
	}

	CollisionInfo info = CircleToCircleCollision(mCircles[0], mCircles[1]);

	lines->DrawText(std::to_string(info.overlapAmount), Vec2{ cursorPos.x, cursorPos.y + 1 }, 1);
	if (info.bIsOverlapping) { mCircles[0]->SetColour(Colour::RED); info.Resolve(); }
	else mCircles[0]->SetColour(Colour::GREEN);
}

void PhysicsEngine::OnLeftClick()
{
}

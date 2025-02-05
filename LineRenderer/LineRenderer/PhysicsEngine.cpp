#include "PhysicsEngine.h"
#include "CollisionFunctions.h"
#include "CollisionInfo.h"
#include "LineRenderer.h"


PhysicsEngine::PhysicsEngine()
{
	//appInfo.fixedFramerate = 5;
	appInfo.appName = "Physics Engine";
}

void PhysicsEngine::Initialise()
{

	// Draw Circle on mouse pos
	mCircles.push_back(new Circle(cursorPos, 1, 0.5f));

	// Create a circle to add to test against
	//mCircles.push_back(new Circle(Vec2{ 1,10 }, 10, 1));
	//mCircles.push_back(new Circle(Vec2{ 10,1 }, 1, 1));
	//mCircles.push_back(new Circle(Vec2{ 4,7 }, 16, 4));

	// Draw boxes
	mBoxes.push_back(new Box(Vec2(1.5, 2.1), 1, 2.2, 1));
	//mBoxes.push_back(new Box(Vec2(2.25, 3.5), 1.5, 1, 1));


	mPlanes[0] = new Plane(Vec2(0,1), 1, 5);
}

void PhysicsEngine::Update(float delta)
{
	mCircles[0]->SetPos(cursorPos);

	for (Circle* circ : mCircles)
	{
		circ->Draw(lines);
	}

	for (Box* b : mBoxes)
	{
		b->Draw(lines);
	}

	for (Plane* p : mPlanes)
	{
		p->Draw(lines);
	}

	for (int i = 0; i < mCircles.size() - 1; i++)
	{
		for (int j = i + 1; j < mCircles.size(); j++)
		{
			CollisionInfo info = CircleToCircleCollision(mCircles[i], mCircles[j]);
			//lines->DrawText(std::to_string(info.overlapAmount), Vec2{ cursorPos.x, cursorPos.y + 1 }, 1);
			//if (info.bIsOverlapping) { mCircles[0]->SetColour(Colour::RED); info.Resolve(); }
			if (info.bIsOverlapping) { mCircles[j]->SetColour(Colour::RED); info.Resolve(); }
			else mCircles[j]->SetColour(Colour::GREEN);
		}
	}

	CollisionInfo info = CircleToBoxCollision(mCircles[0], mBoxes[0]);
	lines->DrawText(std::to_string(info.overlapAmount), Vec2{ cursorPos.x, cursorPos.y + 1 }, 0.5);
	lines->DrawCircle(info.closestPoint, 0.1, Colour::YELLOW);
	if (info.bIsOverlapping)
	{ 
		mCircles[0]->SetColour(Colour::RED);
		info.Resolve();

	}
	else mCircles[0]->SetColour(Colour::GREEN);
}


void PhysicsEngine::OnLeftClick()
{
}

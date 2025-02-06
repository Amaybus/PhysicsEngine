#include "PhysicsEngine.h"
#include "CollisionFunctions.h"
#include "CollisionInfo.h"
#include "LineRenderer.h"


PhysicsEngine::PhysicsEngine()
{
	appInfo.appName = "Physics Engine";
}

void PhysicsEngine::Initialise()
{

	// Draw Circle on mouse pos
	mCircles.push_back(new Circle(cursorPos, 1, 0.5f));

	// Create a circle to add to test against
	mCircles.push_back(new Circle(Vec2{ 1,10 }, 10, 1));
	//mCircles.push_back(new Circle(Vec2{ 10,1 }, 1, 1));
	//mCircles.push_back(new Circle(Vec2{ 4,7 }, 16, 4));

	// Draw boxes
	mBoxes.push_back(new Box(Vec2(1, 10), 2.1, 1.2, 1));
	//mBoxes.push_back(new Box(Vec2(2.25, 3.5), 1.5, 1, 1));


	mPlanes[0] = new Plane(Vec2(0,1), 2, 5);

	// Draw polygon
	mPolygons.push_back(new Polygon(Vec2(1, 1), 4, 1));
	mPolygons.push_back(new Polygon(Vec2(1, 7), 3, 1));
}

void PhysicsEngine::Update(float delta)
{
	mCircles[0]->SetPos(cursorPos);
	//mBoxes[0]->SetPos(cursorPos);
	//mBoxes[0]->SetPos(Vec2(mBoxes[0]->GetPos().x, mBoxes[0]->GetPos().y-1));

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

	for(Polygon* p : mPolygons)
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

	//CollisionInfo info = CircleToBoxCollision(mCircles[0], mBoxes[0]);
	//lines->DrawText(std::to_string(info.overlapAmount), Vec2{ cursorPos.x, cursorPos.y + 1 }, 0.5);
	//lines->DrawCircle(info.closestPoint, 0.1, Colour::YELLOW);
	//if (info.bIsOverlapping)
	//{ 
	//	mCircles[0]->SetColour(Colour::RED);
	//	info.Resolve();

	//}
	//else mCircles[0]->SetColour(Colour::GREEN);

	CollisionInfo info2 = PlaneToCircleCollision(mPlanes[0], mCircles[1]);
	lines->DrawText(std::to_string(info2.overlapAmount), Vec2{ cursorPos.x, cursorPos.y + 1 }, 0.5);
	if (info2.bIsOverlapping)
	{
		mCircles[1]->SetColour(Colour::RED);
		info2.Resolve();

	}
	else mCircles[1]->SetColour(Colour::GREEN);



	//CollisionInfo info1 = PlaneToBoxCollision(mPlanes[0], mBoxes[0], lines);
	////lines->DrawText(std::to_string(info1.overlapAmount), Vec2{ cursorPos.x, cursorPos.y + 1 }, 0.5);
	//lines->DrawCircle(info1.closestPoint, 0.1, Colour::YELLOW);
	//if (info1.bIsOverlapping)
	//{
	//	mBoxes[0]->SetColour(Colour::RED);
	//	info1.Resolve();

	//}
	//else mBoxes[0]->SetColour(Colour::GREEN);
}


void PhysicsEngine::OnLeftClick()
{
}

#include "PhysicsEngine.h"

#include <iostream>

#include "CollisionFunctions.h"
#include "CollisionInfo.h"
#include "imgui.h"
#include "LineRenderer.h"
#include "Circle.h"
#include "Box.h"
#include "Plane.h"
#include "Polygon.h"



PhysicsEngine::PhysicsEngine()
{
	//appInfo.fixedFramerate = 5;
	appInfo.appName = "Physics Engine";
}

void PhysicsEngine::Initialise()
{

	// Draw Circle on mouse pos
	//mPhysicsObjects.push_back(new Circle(Vec2{ -4,0 }, 0.5f, 1));
	//mPhysicsObjects[0]->SetVelocity(Vec2(1.3f, 0));
	// Create a circle to add to test against
	//mPhysicsObjects.push_back(new Circle(Vec2{ 4,5 }, 0.3,1));
	//mCircles[1]->SetVelocity(Vec2(-1, 0));
	//mCircles.push_back(new Circle(Vec2{ 1.3,1 }, 0.3, 1));
	//mCircles.push_back(new Circle(Vec2{ 1.6,1 }, 0.3, 1));

	// Draw boxes
	//mPhysicsObjects.push_back(new Box(Vec2(-0.4f, 3), 2.1f, 1.2f, 2.1f * 1.2f));
	//mPhysicsObjects[1]->SetVelocity(Vec2(0.3f, -1));
	//mPhysicsObjects[0]->ApplyForce(Vec2(-50, 0));

	//mPhysicsObjects.push_back(new Box(Vec2(-0.4f, 0), 7.4f, 0.3f, 70));
	//mPhysicsObjects[1]->SetVelocity(Vec2(0.5f, 2));
	
	//mPhysicsObjects.push_back(new Circle(Vec2(-0.4f, 0), 0.4f, 3.14159f * (0.4f * 0.4f)));
	//mPhysicsObjects[2]->SetVelocity(Vec2(-0.1f, 4));

	//mPhysicsObjects.push_back(new Box(Vec2(-0.4f, 0), 1.5, 1, 1));
	//mPhysicsObjects[1]->SetVelocity(Vec2(0, 0.9f));

	//mPlanes.push_back(new Plane(Vec2(1, 0), -5, 5));
	//mPhysicsObjects.push_back(new Plane(Vec2(-1, 0), -5.4, 5));
	mPhysicsObjects.push_back(new Plane(Vec2(0, 1), -5, 50));
	mPhysicsObjects.push_back(new Plane(Vec2(0, -1), -5, 50));
	mPhysicsObjects.push_back(new Plane(Vec2(1, 0), -5, 50));
	mPhysicsObjects.push_back(new Plane(Vec2(-1, 0), -5, 50));

	// Draw polygon
	mPhysicsObjects.push_back(new Polygon(Vec2(1, 1), 4, 1));
	mPhysicsObjects.push_back(new Polygon(Vec2(1, 7), 3, 1));
	CollisionFuncInit();

}

void PhysicsEngine::Update(float delta)
{
	mPhysicsObjects[5]->SetPos(cursorPos);
	for (PhysicsObject* obj : mPhysicsObjects)
	{
		obj->Update(delta);
	}

	for (int i = 0; i < mPhysicsObjects.size() - 1; i++)
	{
		for (int j = i + 1; j < mPhysicsObjects.size(); j++)
		{
			CollisionInfo info = CheckCollision(mPhysicsObjects[i], mPhysicsObjects[j]);
			if (info.objA != nullptr && info.objB != nullptr)
			{
				if (info.bIsOverlapping)
				{
					info.Resolve();
					info.objB->SetColour(Colour::RED);
					lines->DrawCross(info.contactPoint, 0.2, Colour::BLUE);
				}
				else info.objB->SetColour(Colour::GREEN);
			}
		}
	}

	for (PhysicsObject* o : mPhysicsObjects)
	{
		if (leftMouseDown)
		{
			int t = o->GetType();
			if(t != 3) { continue;}

			Polygon* poly = (Polygon*)o;
			if (poly != nullptr)
			{
				for(int i = 0; i < poly->mVertices.size(); i++)
				if ((poly->mVertices[i] - cursorPos).GetMagnitudeSquared() < 0.5)
				{
					poly->mVertices[i] = (cursorPos);
				}
			}
		}
	}

	lines->DrawLineWithArrow(mPhysicsObjects[0]->GetPos(), mPhysicsObjects[0]->GetPos() + mPhysicsObjects[0]->GetVelocity());
	lines->DrawLineWithArrow(mPhysicsObjects[1]->GetPos(), mPhysicsObjects[1]->GetPos() + mPhysicsObjects[1]->GetVelocity());
	lines->DrawLineWithArrow(mPhysicsObjects[2]->GetPos(), mPhysicsObjects[2]->GetPos() + mPhysicsObjects[2]->GetVelocity());
	for (PhysicsObject* obj : mPhysicsObjects)
	{
		obj->Draw(lines);
	}
}


void PhysicsEngine::OnLeftClick()
{
}

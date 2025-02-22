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
	CollisionFuncInit();

	// Create circles	
	// RIGHT SIDE						   
	mPhysicsObjects.push_back(new Circle(Vec2(1, 0), 0.5, 1));
	mPhysicsObjects[0]->SetVelocity(Vec2(5,0.6));

	mPhysicsObjects.push_back(new Circle(Vec2(5, 00), 0.5, 1));
	mPhysicsObjects[1]->SetVelocity(Vec2(5, 0.1));
	
	mPhysicsObjects.push_back(new Circle(Vec2(-1, 5.0f), 0.5, 1));
	mPhysicsObjects[2]->SetVelocity(Vec2(5, 0));

	// LEFT SIDE
	//mPhysicsObjects.push_back(new Circle(Vec2(-1, 0), 0.5, 1));
	//mPhysicsObjects[0]->SetVelocity(Vec2(1, 0));
	//
	//mPhysicsObjects.push_back(new Circle(Vec2(5, -5.7), 0.5, 1));
	//mPhysicsObjects[1]->SetVelocity(Vec2(-1, 0));
	//
	//mPhysicsObjects.push_back(new Circle(Vec2(-1, 5.0f), 0.5, 1));
	//mPhysicsObjects[5]->SetVelocity(Vec2(-1, 0));



	// Create boxes
	// RIGHT SIDE
	//mPhysicsObjects.push_back(new Box(Vec2(0, -0.9), 0.7f, 3.0f, 1));
	//mPhysicsObjects[0]->SetVelocity(Vec2(1, 1));	
	//mPhysicsObjects.push_back(new Box(Vec2(9, 2), 0.7f, 3.0f, 1));
	//mPhysicsObjects[1]->SetVelocity(Vec2(1.3, 0.5));
	
	//mPhysicsObjects.push_back(new Box(Vec2(3, 4), 0.7f, 3.0f, 2.1f * 1.2f));


	// LEFT SIDE
	//mPhysicsObjects.push_back(new Box(Vec2(-6, 0), 0.7f, 3.0f, 2.1f * 1.2f));
	////mPhysicsObjects[1]->SetVelocity(Vec2(-1, 0));	
	//
	//mPhysicsObjects.push_back(new Box(Vec2(-6, -5.0), 0.7f, 3.0f, 2.1f * 1.2f));
	////mPhysicsObjects[1]->SetVelocity(Vec2(-1, 0));
	//
	//mPhysicsObjects.push_back(new Box(Vec2(-6, 4), 0.7f, 3.0f, 2.1f * 1.2f));


	mPhysicsObjects.push_back(new Polygon(Vec2(-5, 1.3), 7, 1));
	mPhysicsObjects[3]->SetVelocity(Vec2(5, 1));
	mPhysicsObjects.push_back(new Polygon(Vec2(3, 1), 5, 1));
	mPhysicsObjects[4]->SetVelocity(Vec2(5,0 ));
	// Create polygon
	//

	mPhysicsObjects.push_back(new Plane(Vec2(0, 1), -5, 50));
	mPhysicsObjects.push_back(new Plane(Vec2(0, -1), -5, 50));
	mPhysicsObjects.push_back(new Plane(Vec2(1, 0), -5, 50));
	mPhysicsObjects.push_back(new Plane(Vec2(-1, 0), -5, 50));
}

void PhysicsEngine::Update(float delta)
{
	//mPhysicsObjects[1]->SetPos(cursorPos);

	for (PhysicsObject* obj : mPhysicsObjects)
	{
		obj->Update(delta);
		obj->SetColour(Colour::GREEN);
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
					info.ResolveRotation();
					//info.objB->SetColour(Colour::RED);
					info.objA->SetColour(Colour::RED);
					for(Vec2& v : info.bContactPoints)
					{
						lines->DrawCross(v, 0.2, Colour::BLUE);
					}
					for(Vec2& v : info.aContactPoints)
					{
						lines->DrawCross(v, 0.1, Colour::MAGENTA);
					}
				}
			}
		}
	}

	for (PhysicsObject* obj : mPhysicsObjects)
	{
		obj->Draw(lines);
	}
}


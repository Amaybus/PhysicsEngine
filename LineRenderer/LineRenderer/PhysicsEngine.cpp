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
	appInfo.appName = "Physics Engine";
}

PhysicsEngine::~PhysicsEngine()
{
	for(PhysicsObject* po : mPhysicsObjects)
	{
		delete po;
	}
}

void PhysicsEngine::Initialise()
{
	CollisionFuncInit();

	// Create circles	
	mPhysicsObjects.push_back(new Circle(Vec2(1, 0), 0.5, 1));
	mPhysicsObjects[0]->SetVelocity(Vec2(-5,0.6));

	mPhysicsObjects.push_back(new Circle(Vec2(5, 00), 1,2));
	mPhysicsObjects[1]->SetVelocity(Vec2(4, 4));
	
	mPhysicsObjects.push_back(new Circle(Vec2(-1, 5.0f), 0.5, 1));
	mPhysicsObjects[2]->SetVelocity(Vec2(4, -1));

	// Create boxes
	//mPhysicsObjects.push_back(new Box(Vec2(0, -0.9), 0.7f, 2.4f, 1));
	//mPhysicsObjects[3]->SetVelocity(Vec2(-1, 1));	
	//mPhysicsObjects.push_back(new Box(Vec2(9, 2), 1.2f, 3.0f, 1));
	//mPhysicsObjects[4]->SetVelocity(Vec2(1.3, -1));

	// Create polygons
	mPhysicsObjects.push_back(new Polygon(Vec2(-5, 1.3), 7, 1));
	mPhysicsObjects[3]->SetVelocity(Vec2(5, 1));

	mPhysicsObjects.push_back(new Polygon(Vec2(3, 1), 5, 1));
	mPhysicsObjects[4]->SetVelocity(Vec2(-5,0 ));

	mPhysicsObjects.push_back(new Polygon(Vec2(3, 1), 5,1));
	mPhysicsObjects[5]->SetVelocity(Vec2(5,0 ));

	mPhysicsObjects.push_back(new Polygon(Vec2(3, 1), 4,1));
	mPhysicsObjects[6]->SetVelocity(Vec2(4,-3 ));

	mPhysicsObjects.push_back(new Polygon(Vec2(3, 1), 4,1));
	mPhysicsObjects[7]->SetVelocity(Vec2(-3,2));

	mPhysicsObjects.push_back(new Polygon(Vec2(3, 1), 9,1));
	mPhysicsObjects[8]->SetVelocity(Vec2(2,-4 ));

	// Create planes
	mPhysicsObjects.push_back(new Plane(Vec2(0, 1), -10));
	mPhysicsObjects.push_back(new Plane(Vec2(0, -1), -10));
	mPhysicsObjects.push_back(new Plane(Vec2(1, 0), -10));
	mPhysicsObjects.push_back(new Plane(Vec2(-1, 0), -10));
}

void PhysicsEngine::Update(float delta)
{
	for (PhysicsObject* obj : mPhysicsObjects)
	{
		obj->Update(delta);
		obj->SetColour(Colour::GREEN);
	}

	// Check for collisions
	for (int i = 0; i < mPhysicsObjects.size() - 1; i++)
	{
		for (int j = i + 1; j < mPhysicsObjects.size(); j++)
		{
			CollisionInfo info = CheckCollision(mPhysicsObjects[i], mPhysicsObjects[j]);

			if (info.objA != nullptr && info.objB != nullptr)
			{
				if (info.bIsOverlapping)
				{
					info.ResolveWithRotation();
					info.objB->SetColour(Colour::RED);
					info.objA->SetColour(Colour::RED);
				}
			}
		}
	}

	for (PhysicsObject* obj : mPhysicsObjects)
	{
		obj->Draw(lines);
	}
}


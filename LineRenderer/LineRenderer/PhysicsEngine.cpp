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

	//mPhysicsObjects.push_back(new Polygon(Vec2(0, 0), 6,1));
	////mPhysicsObjects[0]->SetOrientation(45);
	//mPhysicsObjects.push_back(new Polygon(Vec2(2.5, 0), 6,1));
	////mPhysicsObjects[1]->SetOrientation(45);
	//mPhysicsObjects.push_back(new Polygon(Vec2(4.5, 0), 6,1));
	////mPhysicsObjects[2]->SetOrientation(45);
	//mPhysicsObjects.push_back(new Polygon(Vec2(-2.5, 0), 6,1));
	////mPhysicsObjects[3]->SetOrientation(45);
	//mPhysicsObjects[3]->SetVelocity(Vec2(1,0));


	// Create circles	
	//mPhysicsObjects.push_back(new Circle(Vec2(0, 4), 0.5, 1));
	//mPhysicsObjects[0]->SetVelocity(Vec2(0,1));
	//
	//mPhysicsObjects.push_back(new Circle(Vec2(1, 3), 0.5,1));
	//mPhysicsObjects[1]->SetVelocity(Vec2(0, 1));
	//
	//mPhysicsObjects.push_back(new Circle(Vec2(-1, 5.0f), 0.5, 1));
	//mPhysicsObjects[2]->SetVelocity(Vec2(4, -1));
	////
	////// Create boxes
	//mPhysicsObjects.push_back(new Box(Vec2(0, -0.9), 0.7f, 2.4f, 1));
	////mPhysicsObjects.push_back(new Box(Vec2(9, 2), 1.2f, 3.0f, 1));
	//mPhysicsObjects[0]->SetVelocity(Vec2(2, 0));
	////
	////// Create polygons
	//mPhysicsObjects.push_back(new Polygon(Vec2(-5, 1.3), 7, 1));
	////mPhysicsObjects[2]->SetVelocity(Vec2(5, 1));
	////
	//mPhysicsObjects.push_back(new Polygon(Vec2(3, 1), 5, 1));
	//mPhysicsObjects[3]->SetVelocity(Vec2(-5,0 ));
	////
	//mPhysicsObjects.push_back(new Polygon(Vec2(3, 1), 5,1));
	//mPhysicsObjects[4]->SetVelocity(Vec2(5,0 ));
	////
	//mPhysicsObjects.push_back(new Polygon(Vec2(3, 1), 4,1));
	//mPhysicsObjects[5]->SetVelocity(Vec2(4,-3 ));
	////
	//mPhysicsObjects.push_back(new Polygon(Vec2(3, 1), 4,1));
	//mPhysicsObjects[6]->SetVelocity(Vec2(-3,2));
	//
	//
	////mPhysicsObjects.push_back(new Polygon(Vec2(0, 5), 6,1));
	////mPhysicsObjects[0]->SetVelocity(Vec2(0,0 ));
	//
	//mPhysicsObjects.push_back(new Polygon(Vec2(0, 1), 6,1));
	//mPhysicsObjects[7]->SetVelocity(Vec2(0,0));
	//
	////// Create planes
	mPhysicsObjects.push_back(new Plane(Vec2(0, 1), -5));
	mPhysicsObjects.push_back(new Plane(Vec2(0, -1), -5));
	mPhysicsObjects.push_back(new Plane(Vec2(1, 0), -5));
	mPhysicsObjects.push_back(new Plane(Vec2(-1, 0), -5));

	//mPhysicsObjects.push_back(new Box(Vec2(0, -0.9), 1000.0f, 0.5f, 1));
	//mPhysicsObjects[2]->SetIsKinematic(false);
	//mPhysicsObjects[6]->SetOrientation(45);
}

void PhysicsEngine::Update(float delta)
{
	for (PhysicsObject* obj : mPhysicsObjects)
	{
		obj->Update(delta);
		obj->SetColour(Colour::GREEN);
		//obj->ApplyForce(mGravity);
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


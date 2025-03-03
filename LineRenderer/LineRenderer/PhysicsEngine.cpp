#include "PhysicsEngine.h"

#include "CollisionFunctions.h"
#include "CollisionInfo.h"
#include "LineRenderer.h"
#include "Circle.h"
#include "Box.h"
#include "Plane.h"
#include "Polygon.h"


PhysicsEngine::PhysicsEngine()
{
	appInfo.appName = "Physics Engine";
	appInfo.grid.show = false;
}

PhysicsEngine::~PhysicsEngine()
{
	for (PhysicsObject* po : mPhysicsObjects)
	{
		delete po;
	}
}

void PhysicsEngine::Initialise()
{
	CollisionFuncInit();

	//mPhysicsObjects.push_back(new Plane(Vec2(-1, 1), -4));
	mPhysicsObjects.push_back(new Box(Vec2(1, -5), 7.0f, 0.5f, 1));
	mPhysicsObjects[0]->SetIsKinematic(false);
	mPhysicsObjects[0]->SetOrientation(0);
	
	mPhysicsObjects.push_back(new Box(Vec2(-5, 0), 5.0f, 0.5f, 1));
	mPhysicsObjects[1]->SetIsKinematic(false);
	mPhysicsObjects[1]->SetOrientation(0);



	//mPhysicsObjects.push_back(new Polygon(Vec2(0, 0), 5,1));
	////mPhysicsObjects[1]->SetOrientation(-45);
	//
	//mPhysicsObjects.push_back(new Polygon(Vec2(0, 2.5), 6,1));
	//mPhysicsObjects.push_back(new Polygon(Vec2(0, 4.5), 6,1));
	//mPhysicsObjects.push_back(new Polygon(Vec2(0, -2.5), 6,1));


	// Create circles	
	//mPhysicsObjects.push_back(new Circle(Vec2(-6, -4), 0.5, 1));
	//mPhysicsObjects[4]->SetVelocity(Vec2(1, 0));
	//mPhysicsObjects[0]->SetAsTrigger(true);
	//
	//mPhysicsObjects.push_back(new Circle(Vec2(1, 3), 0.5, 1));
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
	mPhysicsObjects.push_back(new Plane(Vec2(0, 1), -10));
	//mPhysicsObjects.push_back(new Plane(Vec2(0, -1), -10));
	//mPhysicsObjects.push_back(new Plane(Vec2(1, 0), -10));
	//mPhysicsObjects.push_back(new Plane(Vec2(-1, 0), -10));

}

void PhysicsEngine::Update(float delta)
{
	for (PhysicsObject* obj : mPhysicsObjects)
	{
		obj->Update(delta);
		obj->ApplyForce(mGravity);
	}

	// Check for collisions
	for (int i = 0; i < mPhysicsObjects.size() - 1; i++)
	{
		for (int j = i + 1; j < mPhysicsObjects.size(); j++)
		{
			CollisionInfo info = CheckCollision(mPhysicsObjects[i], mPhysicsObjects[j]);

			if (info.objA == nullptr || info.objB == nullptr) { continue; }
			if (info.objA->IsTrigger() || info.objB->IsTrigger()) { info.bIsTrigger = true; }

			if (info.bIsOverlapping)
			{	
				info.ResolveWithRotation();
				continue;
			}

			// Exit trigger
			if (info.bIsTrigger)
			{
				if (info.objA->IsTrigger()) { info.objA->OnEndOverlap(info.objB); }

				if (info.objB->IsTrigger()) { info.objB->OnEndOverlap(info.objA); }

				continue;
			}

			// Exit collision
			if (!info.bIsTrigger)
			{
				info.objA->OnCollisionExit(info.objB);
				info.objB->OnCollisionExit(info.objA);
				continue;
			}
		}
	}

	for (PhysicsObject* obj : mPhysicsObjects)
	{
		obj->Draw(lines);
	}
}




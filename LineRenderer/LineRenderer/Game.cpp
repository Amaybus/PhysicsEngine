#include "Game.h"

#include "Box.h"
#include "Circle.h"
#include "CollisionFunctions.h"

Game::Game()
{
	appInfo.appName = "Some Game Title";
	//appInfo.grid.show = false;
}

Game::~Game()
{
	for (PhysicsObject* po : mPhysicsObjects)
	{
		delete po;
	}

	for (PhysicsObject* so : mStaticObjects)
	{
		delete so;
	}

	for (PhysicsObject* to : mTargets)
	{
		delete to;
	}
}

void Game::Initialise()
{
	CollisionFuncInit();
	SetStaticObjects();
	SetTargets();
}

void Game::Update(float delta)
{
	for (PhysicsObject* obj : mPhysicsObjects)
	{
		obj->Update(delta);
		obj->ApplyForce(mGravity);
	}

	for (PhysicsObject* statObj : mStaticObjects)
	{
		statObj->Update(delta);
	}

	for (PhysicsObject* to : mTargets)
	{
		to->Update(delta);
	}

	Draw(lines);
}

void Game::SetStaticObjects()
{
	mStaticObjects.push_back(new Box(Vec2(9, 8), 8, 0.5, 1));
	mStaticObjects.push_back(new Box(Vec2(13.25, 5.75), 0.5, 5, 1));
	mStaticObjects.push_back(new Box(Vec2(17, 3), 8, 0.5, 1));
	mStaticObjects.push_back(new Box(Vec2(19, 8), 4, 0.5, 1));
	mStaticObjects.push_back(new Box(Vec2(12, 14.5), 6, 0.5, 1));
	mStaticObjects.push_back(new Box(Vec2(23, 10.5), 7, 0.5, 1));
	mStaticObjects[5]->SetOrientation(45);
	mStaticObjects.push_back(new Box(Vec2(8, 4), 5, 0.5, 1));

	for(PhysicsObject* so : mStaticObjects)
	{
		so->SetIsKinematic(false);
	}
}

void Game::SetKinematicObjects()
{
}

void Game::SetTargets()
{
	mTargets.push_back(new Circle(Vec2(8, 4.5), 0.25, 1));
	mTargets.push_back(new Circle(Vec2(8, 8.5), 0.25, 1));
	mTargets.push_back(new Circle(Vec2(20, 3.5), 0.25, 1));
	mTargets.push_back(new Circle(Vec2(19, 8.5), 0.25, 1));
	mTargets.push_back(new Circle(Vec2(12, 15), 0.25, 1));
}

void Game::CheckForCollisions()
{
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
}

void Game::Draw(LineRenderer* lines)
{
	for (PhysicsObject* obj : mPhysicsObjects)
	{
		obj->Draw(lines);
	}

	for( PhysicsObject* statObj : mStaticObjects)
	{
		statObj->Draw(lines);
	}

	for (PhysicsObject* to : mTargets)
	{
		to->Draw(lines);
	}
}

void Game::OnLeftRelease()
{
	// shoot projectile
}

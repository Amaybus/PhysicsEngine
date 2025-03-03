#include "Game.h"
#include "CollisionFunctions.h"

Game::Game()
{
	appInfo.appName = "Some Game Title";
}

Game::~Game()
{
	for (PhysicsObject* po : mPhysicsObjects)
	{
		delete po;
	}
}

void Game::Initialise()
{
	CollisionFuncInit();
}

void Game::Update(float delta)
{
	for (PhysicsObject* obj : mPhysicsObjects)
	{
		obj->Update(delta);
		obj->ApplyForce(mGravity);
	}

	
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
}

void Game::OnLeftRelease()
{
	// shoot projectile
}

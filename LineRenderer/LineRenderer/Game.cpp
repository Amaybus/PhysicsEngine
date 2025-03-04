#include "Game.h"

#include "Box.h"
#include "Circle.h"
#include "Plane.h"
#include "Polygon.h"
#include "PhysicsObject.h"
#include "CollisionFunctions.h"
//#include "dirent.h"
#include "LineRenderer.h"

Game::Game()
{
	appInfo.appName = "Some Game Title";
	appInfo.grid.show = false;
	cameraHeight = 35.0f;
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
	// Create player
	mPlayer = new Polygon(Vec2(0, 0.6), 3, 1);
	mPlayer->SetOrientation(-30);
	mPlayer->SetIsKinematic(false);
	mPhysicsObjects.push_back(mPlayer);
	
	// Build the level
	CollisionFuncInit();
	SetStaticObjects();
	SpawnRandomKinematicObjects();
	SetTargets();
}

void Game::Update(float delta)
{
	lines->DrawText("Markers collected:", Vec2(-8, -2), 0.5, Colour::WHITE);
	lines->DrawText(std::to_string(mTotalMarkersCollected), Vec2(1.5, -2), 0.51, Colour::YELLOW);
	lines->DrawText("out of", Vec2(2.7, -2), 0.5, Colour::WHITE);
	lines->DrawText(std::to_string(mMarkers.size() + mTotalMarkersCollected), Vec2(6.3, -2), 0.5, Colour::YELLOW);

	for (PhysicsObject* obj : mPhysicsObjects)
	{
		obj->Update(delta);
		// Only apply gravity to kinematic objects
		if (obj->GetKinematic())
		{
			obj->ApplyForce(mGravity);
		}
	}

	if (bDrawProjectionLine)
	{
		lines->DrawLineSegment(Vec2(), cursorPos);
	}

	CheckForCollisions();
	Draw(lines);
}

void Game::SetStaticObjects()
{
	// Level Hard Surfaces
	mStaticObjects.push_back(new Box(Vec2(7, 8), 8, 0.5, 1));
	mStaticObjects.push_back(new Box(Vec2(11.25, 5.75), 0.5, 5, 1));
	mStaticObjects.push_back(new Box(Vec2(15, 3), 8, 0.5, 1));
	mStaticObjects.push_back(new Box(Vec2(17, 8), 4, 0.5, 1));
	mStaticObjects.push_back(new Box(Vec2(10, 14.5), 6, 0.5, 1));
	Box* box = new Box(Vec2(21.65, 10.65), 7, 0.5, 1);
	box->SetOrientation(45);
	mStaticObjects.push_back(box);
	mStaticObjects.push_back(new Box(Vec2(6, 4), 4, 0.5, 1));
	box = new Box(Vec2(2.3, 15), 6, 0.5, 1);
	box->SetOrientation(45);
	mStaticObjects.push_back(box);
	
	mStaticObjects.push_back(new Box(Vec2(-7, 8), 8, 0.5, 1));
	mStaticObjects.push_back(new Box(Vec2(-11.25, 5.75), 0.5, 5, 1));
	mStaticObjects.push_back(new Box(Vec2(-15, 3), 8, 0.5, 1));
	mStaticObjects.push_back(new Box(Vec2(-17, 8), 4, 0.5, 1));
	box = new Box(Vec2(-9, 14.5), 6, 0.5, 1);
	mStaticObjects.push_back(box);
	box = new Box(Vec2(-21.65, 10.65), 7, 0.5, 1);
	box->SetOrientation(-45);
	mStaticObjects.push_back(box);
	mStaticObjects.push_back(new Box(Vec2(-6, 4), 4, 0.5, 1));
	box = new Box(Vec2(-2.3, 15), 6, 0.5, 1);
	box->SetOrientation(-45);
	mStaticObjects.push_back(box);
	
	// PLANES
	mStaticObjects.push_back(new Plane(Vec2(0, 1), 0));
	mStaticObjects.push_back(new Plane(Vec2(0, -1), -21));
	mStaticObjects.push_back(new Plane(Vec2(1, 0), -28));
	mStaticObjects.push_back(new Plane(Vec2(-1, 0), -28));


	for (PhysicsObject* so : mStaticObjects)
	{
		so->SetIsKinematic(false);
		so->SetIgnoreCollisionOfSameType(true);
		mPhysicsObjects.push_back(so);
	}
}

void Game::SpawnRandomKinematicObjects()
{
	mPhysicsObjects.push_back(new Circle(Vec2(15, 19.5), 1, 3, 1));
	mPhysicsObjects.push_back(new Circle(Vec2(-15, 19.5), 1, 3, 1));
	mPhysicsObjects.push_back(new Circle(Vec2(-8, 19.5), 1, 3, 1));
}

void Game::SetTargets()
{
	mMarkers.push_back(new Circle(Vec2(6, 4.5), 0.25, 1));
	mMarkers.push_back(new Circle(Vec2(6, 8.5), 0.25, 1));
	mMarkers.push_back(new Circle(Vec2(18, 3.5), 0.25, 1));
	mMarkers.push_back(new Circle(Vec2(17, 8.5), 0.25, 1));
	mMarkers.push_back(new Circle(Vec2(10, 15), 0.25, 1));

	mMarkers.push_back(new Circle(Vec2(-6, 4.5), 0.25, 1));
	mMarkers.push_back(new Circle(Vec2(-6, 8.5), 0.25, 1));
	mMarkers.push_back(new Circle(Vec2(-18, 3.5), 0.25, 1));
	mMarkers.push_back(new Circle(Vec2(-17, 8.5), 0.25, 1));
	mMarkers.push_back(new Circle(Vec2(-10, 15), 0.25, 1));
	mMarkers.push_back(new Circle(Vec2(0, 13.5), 0.25, 1));

	for (PhysicsObject* to : mMarkers)
	{
		mPhysicsObjects.push_back(to);
		to->SetAsTrigger(true);
	}
}

void Game::CheckForCollisions()
{
	// Check for collisions
	for (int i = 0; i < mPhysicsObjects.size() - 1; i++)
	{
		for (int j = i + 1; j < mPhysicsObjects.size(); j++)
		{
			// Skip over static objects checking against each other
			if(mPhysicsObjects[i]->GetIgnoreCollisionOfSameType() && mPhysicsObjects[j]->GetIgnoreCollisionOfSameType()) { continue;}

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

	for (int i = 0; i < std::size(mProjectiles); i++)
	{
		for (int j = 0; j < mMarkers.size(); j++)
		{
			CollisionInfo info = CheckCollision(mProjectiles[i], mMarkers[j]);
			if (info.bIsOverlapping)
			{
				info.ResolveWithRotation();
				mOverlappedMarkers.push_back(info.objA);
				mOverlappedMarkers.push_back(info.objB);
				mTotalMarkersCollected++;
				continue;
			}

			if (info.objA->IsTrigger())
			{
				info.objA->OnEndOverlap(info.objB);
			}

			if (info.objB->IsTrigger())
			{
				info.objB->OnEndOverlap(info.objA);
			}

		}
	}

	// Remove markers once they have been collided with
	std::vector<PhysicsObject*>::iterator it;
	for (int i = 0; i < mOverlappedMarkers.size(); i++)
	{
		if (!mOverlappedMarkers[i]->IsTrigger()) { continue; }
		it = std::find(mMarkers.begin(), mMarkers.end(), mOverlappedMarkers[i]);
		if (it != mMarkers.end())
		{
			mMarkers.erase(it);
		}

		it = std::find(mPhysicsObjects.begin(), mPhysicsObjects.end(), mOverlappedMarkers[i]);
		if (it != mPhysicsObjects.end())
		{
			mPhysicsObjects.erase(it);
			mProjectileIndex--;
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

void Game::OnLeftClick()
{
	bDrawProjectionLine = true;
}

void Game::OnLeftRelease()
{
	mCursorPosEnd = cursorPos;
	bDrawProjectionLine = false;
	mProjectionVelocity = (Vec2() - mCursorPosEnd) * 2;

	mPhysicsObjects.push_back(new Polygon(Vec2(0, 2), std::rand() % 8 + 3, 1));
	mPhysicsObjects[mPhysicsObjects.size()-1]->ApplyImpulse(mProjectionVelocity);
	mCurrentProjectiles++;

	if(mCurrentProjectiles > mMaxProjectiles)
	{
		// Stop objects colliding with the projectile we are going to delete
		for(PhysicsObject* obj : mPhysicsObjects)
		{
			obj->OnCollisionExit(mPhysicsObjects[mPhysicsObjects.size() - 1 - mMaxProjectiles]);
		}
		// Delete projectile
		mPhysicsObjects.erase(mPhysicsObjects.begin() + mPhysicsObjects.size()-1 - mMaxProjectiles);
		mCurrentProjectiles--;
	}
}


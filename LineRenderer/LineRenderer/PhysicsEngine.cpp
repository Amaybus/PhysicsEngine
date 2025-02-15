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
	//mPhysicsObjects.push_back(new Circle(Vec2(-4, 5), 0.3, 1));
	//mPhysicsObjects[0]->SetVelocity(Vec2(1, 0));
	//mPhysicsObjects.push_back(new Circle(Vec2(4, 5), 0.3, 1));
	//mPhysicsObjects[1]->SetVelocity(Vec2(-1, 0));

	// Create boxes
	mPhysicsObjects.push_back(new Box(Vec2(3, 4.4), 2.1f, 1.2f, 2.1f * 1.2f));
	//mPhysicsObjects[0]->SetVelocity(Vec2(1, 0));

	// Create polygon
	mPhysicsObjects.push_back(new Polygon(Vec2(5, 5), 4, 1));
	//mPhysicsObjects[1]->SetVelocity(Vec2(-1, 0));
	//mPhysicsObjects.push_back(new Polygon(Vec2(5, 2), 3, 1));
	//mPhysicsObjects[3]->SetVelocity(Vec2(-1, 0));

	//mPlanes.push_back(new Plane(Vec2(1, 0), -5, 5));
	//mPhysicsObjects.push_back(new Plane(Vec2(0, 1), -10, 50));
	//mPhysicsObjects.push_back(new Plane(Vec2(0, -1), -10, 50));
	//mPhysicsObjects.push_back(new Plane(Vec2(1, 0), -10, 50));
	//mPhysicsObjects.push_back(new Plane(Vec2(-1, 0), -10, 50));
}

void PhysicsEngine::Update(float delta)
{
	mPhysicsObjects[0]->SetPos(cursorPos);
	ImGui::Begin("normal");
	ImGui::DragInt("normal", &normalIndex, 0, 10);
	ImGui::End();

	for (PhysicsObject* obj : mPhysicsObjects)
	{
		obj->Update(delta);
	}

	for (int i = 0; i < mPhysicsObjects.size() - 1; i++)
	{
		for (int j = i + 1; j < mPhysicsObjects.size(); j++)
		{
			CollisionInfo info = CheckCollision(mPhysicsObjects[i], mPhysicsObjects[j]);


			// ######## POLYGON DEBUGGING ######## 
			lines->DrawLineWithArrow(Vec2(), info.nomrals[normalIndex], Colour::MAGENTA);
			ImGui::Begin("normals");
			ImGui::SliderInt("Polygon Normal Index", &normalIndex, 0, info.nomrals.size()-1);
			ImGui::End();

			ImGui::Begin("Overlap Amount");
			ImGui::DragFloat("overlap amount", &info.overlapAmount, 0, 100);
			ImGui::End();

			for (auto r : info.avalues[normalIndex])
			{
				lines->DrawCircle(r * info.nomrals[normalIndex], 0.1, Colour::BLUE);
			}
			for (auto r : info.bvalues[normalIndex])
			{
				lines->DrawCircle(r * info.nomrals[normalIndex], 0.1, Colour::RED);
			}


			if (info.objA != nullptr && info.objB != nullptr)
			{
				if (info.bIsOverlapping)
				{
					//info.Resolve();
					info.objB->SetColour(Colour::RED);
					info.objA->SetColour(Colour::RED);
					lines->DrawCross(info.contactPoint, 0.2, Colour::BLUE);
				}
				else info.objB->SetColour(Colour::GREEN);
			}
		}
	}

	//lines->DrawLineWithArrow(mPhysicsObjects[0]->GetPos(), mPhysicsObjects[0]->GetPos() + mPhysicsObjects[0]->GetVelocity());
	//lines->DrawLineWithArrow(mPhysicsObjects[1]->GetPos(), mPhysicsObjects[1]->GetPos() + mPhysicsObjects[1]->GetVelocity());
	//lines->DrawLineWithArrow(mPhysicsObjects[2]->GetPos(), mPhysicsObjects[2]->GetPos() + mPhysicsObjects[2]->GetVelocity());
	for (PhysicsObject* obj : mPhysicsObjects)
	{
		obj->Draw(lines);
	}
}


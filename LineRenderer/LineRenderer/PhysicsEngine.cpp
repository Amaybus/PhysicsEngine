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
	//mPhysicsObjects.push_back(new Circle(Vec2(2, 0.8), 0.5, 1));
	//mPhysicsObjects[0]->SetVelocity(Vec2(1,0));	
	//
	//mPhysicsObjects.push_back(new Circle(Vec2(5, 0.6), 0.5, 2));
	//////mPhysicsObjects.push_back(new Circle(Vec2(5, -1), 0.5, 2));
	//mPhysicsObjects[1]->SetVelocity(Vec2(-1, 0));	
	
	//mPhysicsObjects.push_back(new Circle(Vec2(-1, 5.0f), 0.5, 1));
	//mPhysicsObjects[2]->SetVelocity(Vec2(1, 0));

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
	mPhysicsObjects.push_back(new Box(Vec2(-2, -0.9), 0.7f, 3.0f, 1));
	mPhysicsObjects[0]->SetVelocity(Vec2(1, 0));	
	mPhysicsObjects.push_back(new Box(Vec2(3, 2), 0.7f, 3.0f, 1));
	mPhysicsObjects[1]->SetVelocity(Vec2(-1, 0));
	//
	//mPhysicsObjects.push_back(new Box(Vec2(3, 4), 0.7f, 3.0f, 2.1f * 1.2f));


	// LEFT SIDE
	//mPhysicsObjects.push_back(new Box(Vec2(-6, 0), 0.7f, 3.0f, 2.1f * 1.2f));
	////mPhysicsObjects[1]->SetVelocity(Vec2(-1, 0));	
	//
	//mPhysicsObjects.push_back(new Box(Vec2(-6, -5.0), 0.7f, 3.0f, 2.1f * 1.2f));
	////mPhysicsObjects[1]->SetVelocity(Vec2(-1, 0));
	//
	//mPhysicsObjects.push_back(new Box(Vec2(-6, 4), 0.7f, 3.0f, 2.1f * 1.2f));


	//mPhysicsObjects.push_back(new Polygon(Vec2(5, 1), 7, 1));
	//mPhysicsObjects[0]->SetVelocity(Vec2(-1, 0));
	//mPhysicsObjects.push_back(new Polygon(Vec2(0, 1), 4, 1));
	//mPhysicsObjects[1]->SetVelocity(Vec2(1, 0));
	// Create polygon
	//

	//mPhysicsObjects.push_back(new Plane(Vec2(0, 1), -10, 50));
	//mPhysicsObjects.push_back(new Plane(Vec2(0, -1), -10, 50));
	//mPhysicsObjects.push_back(new Plane(Vec2(1, 0), -10, 50));
	//mPhysicsObjects.push_back(new Plane(Vec2(-1, 0), -10, 50));
}

void PhysicsEngine::Update(float delta)
{
	//mPhysicsObjects[0]->SetPos(cursorPos);
	//mPhysicsObjects[0]->SetVelocity(Vec2());
	//ImGui::Begin("Verts");
	//ImGui::SliderInt("Verts", &normalIndex, 0, 7);
	//ImGui::End();

	//ImGui::Begin("normal");
	//ImGui::SliderInt("normal", &normalIndex, 0,10 );
	//ImGui::End();

	//if (ImGui::Begin("normals"))
	//{
	//	ImGui::Text("poly normal index");
	//	ImGui::SameLine();
	//	ImGui::SliderInt("## Polygon Normal Index", &normalIndex, 0, 10);
	//}
	////ImGui::SliderInt("edge", &normalIndex, 0, info.edges.size() - 1);
	//ImGui::End();



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


			// ######## POLYGON DEBUGGING ######## 
			//lines->DrawLineWithArrow(info.normals[normalIndex] + info.objA->GetPos(), info.normals[normalIndex] + info.objA->GetPos()+ info.normals[normalIndex], Colour::MAGENTA);
			//lines->DrawLineWithArrow(Vec2(),Vec2() + info.collisionNormal);
			//lines->DrawLineWithArrow(info.verts[1] + info.objB->GetPos(), info.verts[1] + info.objB->GetPos() + info.normals[1]);
			//lines->DrawLineWithArrow(info.verts[2] + info.objB->GetPos(), info.verts[2] + info.objB->GetPos() + info.normals[2]);
			//lines->DrawLineWithArrow(info.verts[3] + info.objB->GetPos(), info.verts[3] + info.objB->GetPos() + info.normals[3]);
			//lines->DrawLineWithArrow(info.objA->GetPos(), info.objA->GetPos()+ info.contactPoint + info.collisionNormal, Colour::BLUE);

			//ImGui::Begin("Overlap Amount");
			//ImGui::DragFloat("overlap amount", &info.overlapAmount, 0, 100);
			//ImGui::End();

			//for (auto r : info.avalues[normalIndex])
			//{
			//	lines->DrawCircle(r * info.nomrals[normalIndex], 0.1, Colour::BLUE);
			//}
			//for (auto r : info.bvalues[normalIndex])
			//{
			//	lines->DrawCircle(r * info.nomrals[normalIndex], 0.1, Colour::RED);
			//}

			//for (auto f : info.avalues)
			//{
			//	for (auto fg : f)
			//	{
			//		lines->DrawCircle(fg * info.nomrals[normalIndex], 0.1, Colour::GREEN);
			//	}
			//}			
			//
			//for (auto f : info.bvalues)
			//{
			//	for (auto fg : f)
			//	{
			//		lines->DrawCircle(fg * info.nomrals[normalIndex], 0.1, Colour::GREEN);
			//	}
			//}



			if (info.objA != nullptr && info.objB != nullptr)
			{
				if (info.bIsOverlapping)
				{
					info.ResolveRotation();
					//info.Resolve();
					//info.objB->SetColour(Colour::RED);
					info.objA->SetColour(Colour::RED);
					lines->DrawCross(info.contactPoint, 0.2, Colour::BLUE);
					lines->DrawCross(info.contactPoint2, 0.2, Colour::MAGENTA);
					lines->DrawLineWithArrow(info.objA->GetPos(), info.objA->GetPos() + info.forceDirA + info.collisionNormal);
					lines->DrawLineWithArrow(info.objB->GetPos(), info.objB->GetPos() + info.forceDirB + info.collisionNormal);
				}
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


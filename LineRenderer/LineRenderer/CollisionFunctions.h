#pragma once

#include "PhysicsObject.h"
#include "CollisionInfo.h"

#include <functional>



inline std::function<CollisionInfo(PhysicsObject*, PhysicsObject*)> collisionFunctions[(int)ObjectType::Count][(int)ObjectType::Count];
void CollisionFuncInit();

CollisionInfo CheckCollision(PhysicsObject* objA, PhysicsObject* objB);

CollisionInfo CircleToCircleCollision(PhysicsObject* circleA, PhysicsObject* circleB);
CollisionInfo CircleToPlaneCollision(PhysicsObject* circleA, PhysicsObject* planeB);
CollisionInfo CircleToBoxCollision(PhysicsObject* circleA, PhysicsObject* boxB);
CollisionInfo CircleToPolygonCollision(PhysicsObject* circleA, PhysicsObject* polygonB);

CollisionInfo PlaneToCircleCollision(PhysicsObject* planeA, PhysicsObject* circleB);
CollisionInfo PlaneToPlaneCollision(PhysicsObject* planeA, PhysicsObject* planeB);
CollisionInfo PlaneToBoxCollision(PhysicsObject* planeA, PhysicsObject* boxB);
CollisionInfo PlaneToPolygonCollision(PhysicsObject* planeA, PhysicsObject* polygonB);

CollisionInfo BoxToCircleCollision(PhysicsObject* boxA, PhysicsObject* circleB);
CollisionInfo BoxToPlaneCollision(PhysicsObject* boxA, PhysicsObject* planeB);
CollisionInfo BoxToBoxCollision(PhysicsObject* boxA, PhysicsObject* boxB);
CollisionInfo BoxToPolygonCollision(PhysicsObject* boxA, PhysicsObject* polygonB);

CollisionInfo PolygonToCircleCollision(PhysicsObject* polygonA, PhysicsObject* CircleB);
CollisionInfo PolygonToPlaneCollision(PhysicsObject* polygonA, PhysicsObject* planeB);
CollisionInfo PolygonToBoxCollision(PhysicsObject* polygonA, PhysicsObject* boxB);
CollisionInfo PolygonToPolygonCollision(PhysicsObject* polygonA, PhysicsObject* polygonB);




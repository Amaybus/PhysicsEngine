#pragma once

#include "PhysicsObject.h"
#include "Circle.h"
#include "Plane.h"
#include "Box.h"
#include "CollisionInfo.h"


CollisionInfo CircleToCircleCollision(Circle* circleA, Circle* circleB);

CollisionInfo PlaneToCircleCollision(Plane* planeA, Circle* circleB);

CollisionInfo CircleToBoxCollision(Circle* circleA, Box* boxB);

CollisionInfo PlaneToBoxCollision(Plane* planeA, Box* boxB);

CollisionInfo BoxToBoxCollision(Box* boxA, Box* boxB);



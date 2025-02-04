#pragma once

#include "PhysicsObject.h"
#include "Circle.h"
#include "Plane.h"
#include "Box.h"
#include "CollisionInfo.h"


CollisionInfo CircleToCircleCollision(Circle* circleA, Circle* circleB);

CollisionInfo PlaneToCircleCollision(Plane* planeA, Circle* circleB);

CollisionInfo CircleToBox(Circle* circleA, Box* boxA);

CollisionInfo PlaneToBox(Plane* planeA, Box* boxB);

CollisionInfo BoxToBox(Box* boxA, Box* boxB);



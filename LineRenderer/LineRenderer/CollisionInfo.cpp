#include "CollisionInfo.h"

void CollisionInfo::Resolve()
{
	objA->SetPos(objA->GetPos() - overlapAmount * collisionNormal);
	objB->SetPos(objB->GetPos() + overlapAmount * collisionNormal);
}

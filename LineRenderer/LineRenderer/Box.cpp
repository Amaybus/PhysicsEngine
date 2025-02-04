#include "Box.h"

Box::Box(Vec2 pos, float w, float h, float mass) : PhysicsObject(pos, mass), mWidth(w), mHeight(h)
{
	mMax = Vec2((pos.x + 0.5 * w), (pos.y + 0.5 * h));
	mMin = Vec2((pos.x - 0.5 * w), (pos.y - 0.5 * h));
}

void Box::Update(float delta)
{
}

void Box::Draw(LineRenderer* lines)
{
}

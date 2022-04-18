#include "pch.h"
#include "NodeMoveable.h"

void NodeMoveable::setVelocity(sf::Vector2f velocity)
{
	mVelocity = velocity;
}

sf::Vector2f NodeMoveable::velocity() const
{
	return mVelocity;
}

void NodeMoveable::accelerate(sf::Vector2f velocity)
{
	mVelocity += velocity;
}

void NodeMoveable::updateThis(const sf::Time& deltaTime)
{
	move(mVelocity * deltaTime.asSeconds());
}

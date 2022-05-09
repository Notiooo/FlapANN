#include "pch.h"
#include "Bird.h"

#include <SFML/Graphics/RectangleShape.hpp>

Bird::Bird(const sf::Texture& birdTexture)
	: mBird(birdTexture)
{
	mBird.setOrigin(mBird.getLocalBounds().width / 2.f, mBird.getLocalBounds().height / 2.f);
}

void Bird::flap()
{
	if (!mIsKilled)
	{
		setVelocity({ 0.f, -mJumpStrength });
	}
}

void Bird::kill()
{
	mIsKilled = true;
}

bool Bird::isDead() const
{
	return mIsKilled;
}

void Bird::handleThisEvents(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
	{
		if (!mIsKilled)
		{
			flap();
			birdScore -= 0.5f;
		}
	}
}

void Bird::drawThis(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mBird, states);
}

bool Bird::isBirdFalling(const float& fallingThreshold) const
{
	const auto& currentRotation = getRotation();
	return velocity().y - fallingThreshold > 0 && (currentRotation < 45 || currentRotation > (365 - 60));
}

bool Bird::isBirdRaising() const
{
	const auto& currentRotation = getRotation();
	return velocity().y < 0 && (currentRotation > (365 - 45) || currentRotation < 60);
}

float Bird::calculateRotationChange(const float& fallingThreshold) const
{
	static const auto& rotationSpeed = 300.f;

	auto rotationSpeedDirected = 0.f;
	if(isBirdFalling(fallingThreshold))
	{
		rotationSpeedDirected = rotationSpeed;
	}
	else if (isBirdRaising())
	{
		rotationSpeedDirected = -rotationSpeed;
	}
	return rotationSpeedDirected;
}

void Bird::updateRotation(const sf::Time& deltaTime)
{
	static const auto& fallingThreshold = mJumpStrength * 0.7f;
	rotate(calculateRotationChange(fallingThreshold) * deltaTime.asSeconds());
}

void Bird::updateThis(const sf::Time& deltaTime)
{
	NodeMoveable::updateThis(deltaTime);

	// It falls down slowly
	accelerate({ 0.f, 500.f * deltaTime.asSeconds() });
	updateRotation(deltaTime);
	if(!mIsKilled)
	{
		birdScore += deltaTime.asSeconds();
	}
}

sf::FloatRect Bird::getBirdBounds() const
{
	auto birdHitboxSize = sf::Vector2f{
		mBird.getTexture()->getSize().x / 1.5f,
		mBird.getTexture()->getSize().y / 1.5f
	};
	auto birdTextureSizeDifference = sf::Vector2f{
		mBird.getTexture()->getSize().x - birdHitboxSize.x,
		mBird.getTexture()->getSize().y - birdHitboxSize.y
	};
	auto rectLeft = getPosition().x - mBird.getLocalBounds().width / 2.f + birdTextureSizeDifference.x;
	auto rectTop = getPosition().y - mBird.getLocalBounds().width / 2.f + birdTextureSizeDifference.y;
	return {rectLeft, rectTop, birdHitboxSize.x, birdHitboxSize.y};
}

void Bird::loadResources(TextureManager& textureManager)
{
	textureManager.storeResource(Textures_ID::Bird_Orange, "resources/textures/birds/bird_orange.png");
	textureManager.storeResource(Textures_ID::Bird_Blue, "resources/textures/birds/bird_blue.png");
	textureManager.storeResource(Textures_ID::Bird_Red, "resources/textures/birds/bird_red.png");
}

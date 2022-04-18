#include "pch.h"
#include "Bird.h"

Bird::Bird(const sf::Texture& birdTexture)
	: mBird(birdTexture)
{
	mBird.setOrigin(mBird.getLocalBounds().width / 2.f, mBird.getLocalBounds().height / 2.f);
}

void Bird::flap()
{
	setVelocity({ 0.f, -mJumpStrength });
}

void Bird::handleThisEvents(const sf::Event& event)
{
	// Actually it is temporary just to test the bird
	if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
	{
		flap();
	}
}

void Bird::drawThis(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mBird, states);
}

bool Bird::isBirdFalling(const float& fallingThreshold)
{
	const auto& currentRotation = getRotation();
	return velocity().y - fallingThreshold > 0 && (currentRotation < 45 || currentRotation > (365 - 60));
}

bool Bird::isBirdRaising()
{
	const auto& currentRotation = getRotation();
	return velocity().y < 0 && (currentRotation > (365 - 45) || currentRotation < 60);
}

float Bird::calculateRotationChange(const float& fallingThreshold)
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
}

void Bird::loadResources(TextureManager& textureManager)
{
	textureManager.storeResource(Textures_ID::Bird_Orange, "resources/textures/birds/bird_orange.png");
	textureManager.storeResource(Textures_ID::Bird_Blue, "resources/textures/birds/bird_blue.png");
	textureManager.storeResource(Textures_ID::Bird_Red, "resources/textures/birds/bird_red.png");
}

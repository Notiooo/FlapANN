#include "pch.h"
#include "Pipe.h"
#include <SFML/Graphics/RectangleShape.hpp>

Pipe::Pipe(const sf::Texture& pipeTexture, MovePattern movePattern)
	: mPipe(pipeTexture)
	, mCurrentMovePattern(movePattern)
{
	setVelocity({-mPipeSpeed, 0.f});
}

void Pipe::loadResources(TextureManager& textureManager)
{
	textureManager.storeResource(Textures_ID::Pipe_Green, "resources/textures/pipe_green.png");
}

void Pipe::drawThis(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mPipe, states);
}

void Pipe::updateThis(const sf::Time& deltaTime)
{
	NodeMoveable::updateThis(deltaTime);

	setPosition(getPosition() + mCurrentMovePattern.positionDelta(deltaTime));
}

sf::FloatRect Pipe::getPipeBounds() const
{
	return (mPipe.getTransform() * getTransform()).transformRect(mPipe.getGlobalBounds());
}

float Pipe::pipeSpeed()
{
	return mPipeSpeed;
}

void Pipe::makeRed()
{
	mPipe.setColor(sf::Color::Red);
}

void Pipe::makeNormal()
{
	mPipe.setColor(sf::Color::White);
}

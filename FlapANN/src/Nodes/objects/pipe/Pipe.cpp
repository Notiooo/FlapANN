#include "pch.h"
#include "Pipe.h"

Pipe::Pipe(const sf::Texture& pipeTexture, MovePattern movePattern)
	: mPipe(pipeTexture)
	, mCurrentMovePattern(movePattern)
{
	setVelocity({-40.f, 0.f});
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
	return mPipe.getLocalBounds();
}

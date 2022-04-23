#include "pch.h"
#include "Pipe.h"

Pipe::Pipe(const sf::Texture& pipeTexture)
	: mPipe(pipeTexture)
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

sf::FloatRect Pipe::getPipeBounds() const
{
	return mPipe.getLocalBounds();
}

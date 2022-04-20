#include "pch.h"
#include "PipesGenerator.h"
#include <random>

std::random_device PipesGenerator::rndDevice;
std::mt19937 PipesGenerator::engine(rndDevice());

PipesGenerator::PipesGenerator(const TextureManager& textures, const sf::Vector2i& screenSize):
	mTextures(textures),
	mClippingPoint(screenSize.x)
{
	yCoordinate.maxPipeOffset = screenSize.y / 2;
}

sf::Vector2f PipesGenerator::calculateRndPipeOffset() const
{
	const std::uniform_int_distribution generatePipeXPos(xCoordinate.minPipeOffset, xCoordinate.maxPipeOffset);
	const std::uniform_int_distribution generatePipeYPos(yCoordinate.minPipeOffset, yCoordinate.maxPipeOffset);

	auto pipeXOffset = static_cast<float>(generatePipeXPos(engine));
	auto upperPipeHeight = static_cast<float>(generatePipeYPos(engine));

	return {pipeXOffset, upperPipeHeight};
}

float PipesGenerator::getLastPipeXPosition() const
{
	return mPipes.empty() ? 0.f : mPipes.back()->getPosition().x;
}

std::unique_ptr<Pipe> PipesGenerator::createBottomPipe(const sf::Vector2f& offset,
                                                       const float& prevPipeXPos) const
{
	auto [pipeXOffset, upperPipeHeight] = offset;
	auto bottomPipe(std::make_unique<Pipe>(mTextures.getResourceReference(Textures_ID::Pipe_Green)));
	bottomPipe->setPosition({prevPipeXPos + pipeXOffset, upperPipeHeight + offsetBetweenPipes});

	return std::move(bottomPipe);
}

std::unique_ptr<Pipe> PipesGenerator::createUpperPipe(const sf::Vector2f& offset,
                                                      const float& prevPipeXPos) const
{
	auto [pipeXOffset, upperPipeHeight] = offset;
	auto upperPipe(std::make_unique<Pipe>(mTextures.getResourceReference(Textures_ID::Pipe_Green)));
	const sf::FloatRect bounds = upperPipe->getPipeBounds();

	upperPipe->setPosition({prevPipeXPos + pipeXOffset, upperPipeHeight});
	upperPipe->setRotation(180);
	upperPipe->setOrigin(bounds.width, 0);

	return std::move(upperPipe);
}

bool PipesGenerator::isPipeOutOfSight() const
{
	const auto& pipe = mPipes.front();
	return !mPipes.empty() && pipe->getPosition().x + pipe->getPipeBounds().width < 0.f;
}

bool PipesGenerator::isLastPipeInsideWindowFrame() const
{
	return mPipes.empty() || mPipes.back()->getPosition().x < mClippingPoint;
}

void PipesGenerator::generatePipe()
{
	const sf::Vector2f& offset = calculateRndPipeOffset();
	const float& prevPipeXPos = getLastPipeXPosition();

	mPipes.emplace_back(createBottomPipe(offset, prevPipeXPos));
	mPipes.emplace_back(createUpperPipe(offset, prevPipeXPos));
}

void PipesGenerator::deletePipes()
{
	mPipes.pop_front();
}

void PipesGenerator::updatePipesPosition(const sf::Time& deltaTime) const
{
	for (const auto& pipe : mPipes)
	{
		pipe->update(deltaTime);
	}
}

void PipesGenerator::updateThis(const sf::Time& deltaTime)
{
	if (isLastPipeInsideWindowFrame())
	{
		generatePipe();
	}
	if (isPipeOutOfSight())
	{
		deletePipes();
	}
	updatePipesPosition(deltaTime);
}

void PipesGenerator::drawThis(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (auto& pipe : mPipes)
	{
		pipe->draw(target, states);
	}
}

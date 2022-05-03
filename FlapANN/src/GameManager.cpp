#include "pch.h"
#include "GameManager.h"

#include "Game.h"
#include "nodes/objects/bird/Bird.h"

GameManager::GameManager(const TextureManager& textureManager, sf::Vector2u screenSize, const FontManager& fonts) :
	mBackground(textureManager),
	mGround(textureManager),
	mPipesGenerator(textureManager, fonts, screenSize)
{
	addBirds(textureManager, screenSize);
	mGround.setPosition(0, static_cast<float>(screenSize.y));
}

void GameManager::update(const sf::Time& deltaTime)
{
	mBackground.update(deltaTime);
	mGround.update(deltaTime);
	mPipesGenerator.update(deltaTime);

	for (auto& bird : mBirds)
	{
		bird.update(deltaTime);
	}
	handleCollision();
}

void GameManager::updateImGui()
{
	mPipesGenerator.updateImGuiThis();
	mBackground.updateImGui();
	mGround.updateImGui();

	for (auto& bird : mBirds)
	{
		bird.updateImGui();
	}
}

void GameManager::handleEvents(const sf::Event& event)
{
	for (auto& bird : mBirds)
	{
		bird.handleEvents(event);
	}
}

void GameManager::handleCollision()
{
	for (auto& bird : mBirds)
	{
		mPipesGenerator.checkCollision(bird);
	}
}

void GameManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mBackground, states);
	target.draw(mPipesGenerator, states);
	target.draw(mGround, states);

	for (const auto& bird : mBirds)
	{
		target.draw(bird, states);
	}
}

void GameManager::addBirds(const TextureManager& textureManager, const sf::Vector2u screenSize,
                                        const unsigned& numberOfBirds)
{
	const auto& birdTextureSize = mBirdTextures.size();

	for (unsigned birdIndex = 0; birdIndex < numberOfBirds; ++birdIndex)
	{
		const int& textureIndex = birdIndex % birdTextureSize;
		mBirds.emplace_back(textureManager.getResourceReference(mBirdTextures[textureIndex]));
		mBirds.back().setPosition({(screenSize.x / 4.f), (screenSize.y / 2.f)});
	}
}

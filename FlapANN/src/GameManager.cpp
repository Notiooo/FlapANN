#include "pch.h"
#include "GameManager.h"

#include <chrono>
#include <imgui/imgui.h>

#include "Game.h"
#include "nodes/objects/bird/Bird.h"

float normalize(float StartRange, float EndRange, float value)
{
	auto oldRange = (EndRange - StartRange);

	constexpr float newMax = 1;
	constexpr float newMin = 0;
	auto newRange = (newMax - newMin);
	return (((value - StartRange) * newRange) / oldRange) + newMin;
}


GameManager::GameManager(const TextureManager& textureManager, sf::Vector2u screenSize, const FontManager& fonts) :
	mBackground(textureManager),
	mGround(textureManager),
	mPipesGenerator(textureManager, fonts, screenSize),
    mTextureManager(textureManager),
    mScreenSize(screenSize),
    mGeneticAlgorithm(100, 15, {2, {8}, 1})
{
	mGround.setPosition(0, static_cast<float>(screenSize.y));
	restartGame();
	mGeneticAlgorithm.createPopulation();
}

bool GameManager::allBirdsDead()
{
    return std::all_of(mBirds.begin(), mBirds.end(), [this](Bird& bird)
    {
        return bird.isDead() && bird.getPosition().x < 0;
    });
}

void GameManager::controlTopScreenBoundaries(Bird& currentBird)
{
    if (currentBird.getPosition().y < 0)
    {
        currentBird.setPosition(currentBird.getPosition().x, 0);
    }
}

void GameManager::controlBottomScreenBoundaries(Bird& currentBird)
{
    static auto groundTop = mScreenSize.y - mTextureManager.getResourceReference(Textures_ID::Ground).getSize().y;
    if (currentBird.getPosition().y + currentBird.getBirdBounds().height > groundTop)
    {
        currentBird.kill();
        currentBird.setPosition(currentBird.getPosition().x, groundTop);
        currentBird.setVelocity({-50.f, 0});
    }
}

void GameManager::controlGameBoundaries(Bird& currentBird)
{
    controlTopScreenBoundaries(currentBird);
    controlBottomScreenBoundaries(currentBird);
}

void GameManager::updateANN()
{
    auto iterator = 0;
    for(auto& currentBird : mBirds)
    {
        auto& currentGenome = mGeneticAlgorithm.at(iterator);
        const auto& nearestPipe = mPipesGenerator.sortedNearestPipeSets(currentBird.getPosition());
        const auto& horizontalDistance = std::clamp(normalize(0, mScreenSize.x, 
                                                   std::abs(currentBird.getPosition().x - nearestPipe.front()->position().x)
        ), 0.f, 1.f);
        const auto& verticalDistance = std::clamp(normalize(0, mScreenSize.y, 
                                                 std::abs(currentBird.getPosition().y - nearestPipe.front()->position().y)
        ), 0.f, 1.f);
        currentGenome.fitness = currentBird.birdScore;
        currentGenome.performOnPredictedOutput({ horizontalDistance, verticalDistance }, [&currentBird](fann_type* output)
        {
            if(output[0] > 0.5f)
            {
                currentBird.flap();
            }
        });
        ++iterator;
    }
}

void GameManager::updateBirds(const sf::Time& deltaTime)
{
    for (auto currentBird = mBirds.begin(), birdEnd = mBirds.end(); currentBird != birdEnd; ++currentBird)
    {
        currentBird->update(deltaTime);
        controlGameBoundaries(*currentBird);
    }
}

void GameManager::update(const sf::Time& deltaTime)
{
	mBackground.update(deltaTime);
	mGround.update(deltaTime);
	mPipesGenerator.update(deltaTime);
	

	updateBirds(deltaTime);
	updateANN();
	handleCollision();
	if (allBirdsDead())
	{
		restartGame();
		mGeneticAlgorithm.evolve();
	}
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

	if (!mBirds.empty())
	{
		static std::vector<float> vertical;
		static std::vector<float> horizontal;
		auto& firstBird = mBirds.front();
		const auto& nearestPipe = mPipesGenerator.sortedNearestPipeSets(firstBird.getPosition());
		if (!nearestPipe.empty())
		{
			const auto& horizontalDistance = std::clamp(normalize(0, mScreenSize.x,
				std::abs(firstBird.getPosition().x - nearestPipe.front()->position().x)
			), 0.f, 1.f);
			const auto& verticalDistance = std::clamp(normalize(0, mScreenSize.y,
				std::abs(firstBird.getPosition().y - nearestPipe.front()->position().y)
			), 0.f, 1.f);
			vertical.push_back(verticalDistance);
			horizontal.push_back(horizontalDistance);
			char overlayHor[32];
            sprintf(overlayHor, "hor: %f", horizontalDistance);
			char overlayVer[32];
            sprintf(overlayVer, "ver: %f", verticalDistance);
			ImGui::PlotLines("Horizontal", horizontal.data(), horizontal.size(), 0, overlayHor);
			ImGui::PlotLines("Vertical", vertical.data(), vertical.size(), 0, overlayVer);
		}
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

void GameManager::restartGame()
{
	mBirds.clear();
	mPipesGenerator.restart();
	addBirds(mTextureManager, mScreenSize, 100);
}

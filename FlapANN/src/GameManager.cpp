#include "pch.h"
#include "GameManager.h"

#include <chrono>
#include <optional>
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
    mGeneticAlgorithm(150, 5, {3, {8}, 1})
{
	mGround.setPosition(0, static_cast<float>(screenSize.y));
	restartGame();
	mGeneticAlgorithm.createPopulation();
}

bool GameManager::allBirdsAreDead()
{
    return std::all_of(mBirds.begin(), mBirds.end(), [this](const Bird& bird)
    {
        return bird.isDead() && bird.getPosition().x < 0;
    });
}

void GameManager::controlTopScreenBoundaries(Bird& currentBird)
{
    if (currentBird.getPosition().y < 0)
    {
		currentBird.kill();
    }
}

void GameManager::controlBottomScreenBoundaries(Bird& currentBird) const
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

float GameManager::horizontalNormalizedDistanceBetweenBirdAndPipeset(const Bird& currentBird, const PipeSet& nearestPipe) const
{
	auto xDelta = currentBird.getPosition().x - nearestPipe.position().x;
	float horizontalDistance = std::clamp(normalize(0, mScreenSize.x,std::abs(xDelta)), 0.f, 1.f);
	horizontalDistance = (xDelta < 0) ? horizontalDistance : -horizontalDistance;
	return horizontalDistance;
}

float GameManager::verticalNormalizedDistanceBetweenBirdAndPipeset(const Bird& currentBird, const PipeSet& nearestPipe) const
{
	auto yDelta = currentBird.getPosition().y - nearestPipe.position().y;
	auto verticalDistance = std::clamp(normalize(0, mScreenSize.y, std::abs(yDelta)), 0.f, 1.f);
	verticalDistance = (yDelta < 0) ? verticalDistance : -verticalDistance;
	return verticalDistance;
}

float GameManager::normalizedVerticalBirdPosition(const Bird& currentBird) const
{
	return std::clamp(normalize(0, mScreenSize.y,
	                            std::abs(currentBird.getPosition().y)), 0.f, 1.f);
}

float GameManager::distance(float x, float y)
{
	return std::sqrtf(std::powf(x, 2) + std::powf(y, 2));
}

float GameManager::calculateBirdFitnessScore(const Bird& currentBird, const float& distanceToGap)
{
	return currentBird.birdScore() - distanceToGap / 10.f;
}

std::pair<float, float> GameManager::normalizedDistancesBetweenBirdAndPipeset(std::list<Bird>::value_type& currentBird, const PipeSet& nearestPipe) const
{
	auto horizontalDistance = horizontalNormalizedDistanceBetweenBirdAndPipeset(currentBird, nearestPipe);
	auto verticalDistance = verticalNormalizedDistanceBetweenBirdAndPipeset(currentBird, nearestPipe);

	return { horizontalDistance, verticalDistance };
}

void GameManager::updateANN()
{
	if(mBirds.size() != mGeneticAlgorithm.population().size())
	{
		throw std::runtime_error("Number of birds is not equal to number of 'brains'");
	}

	auto birdNumber = 0;
    for(auto& currentBird : mBirds)
    {
        const auto& nearestPipe = *mPipesGenerator.sortedByDistancePipesetsInfrontOfBird(currentBird.getPosition()).front();
        const auto& [horizontalDistance, verticalDistance] = normalizedDistancesBetweenBirdAndPipeset(currentBird, nearestPipe);
        const auto& birdPositionY = normalizedVerticalBirdPosition(currentBird);
		const auto& distanceToGap = distance(horizontalDistance, verticalDistance);

		auto& currentGenome = mGeneticAlgorithm.at(birdNumber);
        currentGenome.fitness = calculateBirdFitnessScore(currentBird, distanceToGap);
        currentGenome.performOnPredictedOutput({ horizontalDistance, verticalDistance, birdPositionY }, [&currentBird](fann_type* output)
        {
            if(output[0] > 0.5f)
            {
                currentBird.flap();
            }
        });
        ++birdNumber;
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
	if (allBirdsAreDead())
	{
		restartGame();
		mGeneticAlgorithm.evolve();
	}
}

std::optional<Bird*> GameManager::firstBirdAlive()
{
	const auto& firstAliveBird = 
		std::find_if(mBirds.begin(), mBirds.end(), 
			[](const Bird& bird) { return !bird.isDead(); });

	if (firstAliveBird != mBirds.end())
	{
		return std::make_optional(&*firstAliveBird);
	}
	return std::nullopt;
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

void GameManager::restartGame()
{
	mBirds.clear();
	mPipesGenerator.restart();
	addBirds(mTextureManager, mScreenSize, 150);
}

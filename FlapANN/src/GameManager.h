#pragma once
#include <optional>

#include "GeneticAlgorithm.h"
#include "nodes/objects/background/Background.h"
#include "nodes/objects/background/Ground.h"
#include "nodes/objects/bird/Bird.h"
#include "nodes/objects/pipe/PipesGenerator.h"
#include "EvolutionNet/EvolutionNet.hpp"


struct FlapAnnDataSet
{
	fann_type verticalDistance;
	//fann_type nextPipeOffset;
	fann_type horizontalDistance;
	Bird* birb;
};


class GameManager : public sf::Drawable
{
public:
	/**
	 * \brief The main constructor of the game manager.
	 * \param textureManager Texture manager holds all the available textures in the game.
	 * \param screenSize Holds width and height of the game screen
	 * \param fonts Font manager holds all the available fonts in the game
	 */
	GameManager(const TextureManager& textureManager, sf::Vector2u screenSize, const FontManager& fonts);

    /**
	 * \brief Updates game logic
	 * \param deltaTime the time that has passed since the game was last updated.
	 *
	 * Updates the game logic by passing the time that has elapsed
	 * since the previous update. This allows objects to move independently
	 * of the speed at which subsequent iterations of the program are executed.
	 * (distance = speed * time)
	 */
	void update(const sf::Time& deltaTime);
	std::optional<Bird*> firstBirdAlive();

	/**
	 * \brief Updates the ImGui related code
	 */
	void updateImGui();

	/**
	 * \brief Intercepts player inputs and passes them to processes inside the game.
	 */
	void handleEvents(const sf::Event& event);

	/**
	 * \brief Handles collisions between all objects in the game
	 */
	void handleCollision();

	/**
	 * \brief Draws all drawable objects present in the game to the passed target.
	 * \param target Where it should be drawn to.
	 * \param states Provides information about rendering process (transform, shader, blend mode).
	 */
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	/**
	 * \brief Add a predefined number of birds to the game.
	 * \param textureManager textureManager Texture manager holds all the available textures in the game.
	 * \param screenSize Holds width and height of the game screen.
	 * \param numberOfBirds Birds that will be present in the game.
	 */
	void addBirds(const TextureManager& textureManager, sf::Vector2u screenSize,
	              const unsigned& numberOfBirds = 5);

private:
	void restartGame();
	bool areAllBirdsDead();
	static void controlTopScreenBoundaries(Bird& currentBird);
    void controlBottomScreenBoundaries(Bird& currentBird) const;
    void controlGameBoundaries(Bird& currentBird);
	float horizontalNormalizedDistanceBetweenBirdAndPipeset(const Bird& currentBird, const PipeSet& nearestPipe) const;
	float verticalNormalizedDistanceBetweenBirdAndPipeset(const Bird& currentBird, const PipeSet& nearestPipe) const;
	float normalizedVerticalBirdPosition(const Bird& currentBird) const;
	static float distance(float x, float y);
	static float calculateBirdFitnessScore(const Bird& currentBird, const float& distanceToGap);
	std::pair<float, float> normalizedDistanceBetweenBirdAndPipeset(std::list<Bird>::value_type& currentBird, const PipeSet& nearestPipe) const;
	void updateANN();
    void updateBirds(const sf::Time& deltaTime);

private:
	const TextureManager& mTextureManager;
	sf::Vector2u mScreenSize;

	/** Scrollable background */
	Background mBackground;

	/** Scrollable ground */
	Ground mGround;

	/** Handles pipes generation and related operations */
	PipesGenerator mPipesGenerator;

	/** List of current birds in the game */
	std::list<Bird> mBirds;

	/** Array containing all types of bird textures */
	std::array<Textures_ID, 3> mBirdTextures{Textures_ID::Bird_Blue, Textures_ID::Bird_Orange, Textures_ID::Bird_Red};

	// Testing bro
	GeneticAlgorithm mGeneticAlgorithm;
};

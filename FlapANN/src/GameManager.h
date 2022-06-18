#pragma once
#include <optional>

#include "GeneticAlgorithm.h"
#include "nodes/objects/background/Background.h"
#include "nodes/objects/background/Ground.h"
#include "nodes/objects/bird/Bird.h"
#include "nodes/objects/pipe/PipesGenerator.h"



/**
 * \brief The main manager who manages the running of the game
 */
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
	/**
	 * \brief Restarts the game and adds birds again
	 */
	void restartGame();

	/**
	 * \brief Checks if all birds in the game are already dead
	 * \return True if all birds are dead, false otherwise
	 */
	bool allBirdsAreDead();

	/**
	 * \brief Checks if the bird crosses the top border of the screen.
	 * If it does, kills it.
	 *
	 * \param currentBird A bird that is checked for crossing the top edge of the screen
	 */
	static void killIfExceedsTopScreenBoundary(Bird& currentBird);

	/**
	 * \brief Checks if the bird crosses the bottom border of the screen.
	 * If it does, kills it and imparts a velocity equal to that of the moving floor
	 *
	 * \param currentBird A bird that is checked for crossing the bottom edge of the screen
	 */
    void killIfExceedsBottomScreenBoundary(Bird& currentBird) const;

	/**
	 * \brief Checks if the bird crosses the borders of the screen.
	 * If it does, kills it and imparts a velocity equal to that of the moving floor
	 *
	 * \param currentBird A bird that is checked for crossing the edge of the screen
	 */
    void killIfExceedsScreenBoundaries(Bird& currentBird);

	/**
	 * \brief Gives the horizontal distance between the bird and the pipes normalized to a value between 0 and 1.
	 * \param currentBird Bird from which distance is measured
	 * \param nearestPipe Pipeset from which distance is measured
	 * \return Horizontal distance between bird and between two pipes
	 */
	float horizontalNormalizedDistanceBetweenBirdAndPipeset(const Bird& currentBird, const PipeSet& nearestPipe) const;

	/**
	 * \brief Gives the vertical distance between the bird and the pipe gap normalized to a value between 0 and 1.
	 * \param currentBird Bird from which distance is measured
	 * \param nearestPipe Pipeset from which distance is gained to the middle of the gap between the two pipes
	 * \return Vertical distance between bird and gap between two pipes
	 */
	float verticalNormalizedDistanceBetweenBirdAndPipeset(const Bird& currentBird, const PipeSet& nearestPipe) const;

	/**
	 * \brief The height at which the bird is located normalized to a range of 0 to 1.
	 * \param currentBird Bird whose height is being checked
	 * \return Height in range 0 to 1
	 */
	float normalizedVerticalBirdPosition(const Bird& currentBird) const;

	/**
	 * \brief The distance resulting from the Pythagoras theorem - calculated as the length of the hypotenuse.
	 * \param x The length of side x
	 * \param y The length of side y
	 * \return Length of connecting side
	 */
	static float distance(float x, float y);

	/**
	 * \brief Calculates the bird's earned fitness score
	 * \param currentBird Bird for which the fitness score is calculated
	 * \param distanceToGap The distance between the bird and the nearest gap between the pipes
	 * \return Fitness score of the bird
	 */
	static float calculateBirdFitnessScore(const Bird& currentBird, const float& distanceToGap);

	/**
	 * \brief Normalized vertical and horizontal distance from 0 to 1 between the bird and the nearest gap between two pipes.
	 * \param currentBird Bird for which the distance is counted
	 * \param nearestPipeset Nearest two pipes between which the distance is calculated
	 * \return Normalized horizontal (first) and vertical (second) distance from 0 to 1
	 */
	std::pair<float, float> normalizedDistancesBetweenBirdAndPipeset(const Bird& currentBird, const PipeSet& nearestPipeset) const;

	/**
	 * \brief Updates the state of Artificial Neural Network
	 */
	void updateANN();

	/**
     * \brief Updates the state of the birds in the game
     * \param deltaTime Time elapsed since previous update
     */
    void updateBirds(const sf::Time& deltaTime);

private:
	/** Manager that stores references to textures in the game */
	const TextureManager& mTextureManager;

	/** Size of the screen where the game is displayed */
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

	/** Genetic algorithm used to control bird behavior */
	GeneticAlgorithm mGeneticAlgorithm;
};

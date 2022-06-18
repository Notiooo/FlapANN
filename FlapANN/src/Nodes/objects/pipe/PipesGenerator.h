#pragma once

#include <deque>
#include <memory>
#include "Pipe.h"
#include "PipeSet.h"
#include "nodes/objects/bird/Bird.h"


/**
 * \brief The generator creates pipes based on the information and parameters provided
 */
class PipesGenerator : public NodeScene
{
	/**
	 * \brief Contains data needed to calculate randomized offsets
	 */
	struct PipeOffset
	{
		float minPipeOffset;
		float maxPipeOffset;
	};

public :
	/**
	 * \brief The main constructor of the pipe generator.
	 * \param textures Texture manager holds all the available textures in the game.
	 * \param fonts Font manager holds all the available fonts in the game
	 * \param screenSize Holds width and height of the game screen
	 */
	PipesGenerator(const TextureManager& textures, const FontManager& fonts, const sf::Vector2u& screenSize);

	/**
	 * \brief Updates the logic of the pipe generator. 
	 * \param deltaTime the time that has passed since the game was last updated
	 */
	void updateThis(const sf::Time& deltaTime) override;

	/**
	 * \brief Updates sliders and options related to PipesGenerator
	 */
	void updateImGuiThis() override;

	/**
	 * \brief Draws the pipe to the passed target.
	 * \param target where it should be drawn to
	 * \param states provides information about rendering process (transform, shader, blend mode)
	 */
	void drawThis(sf::RenderTarget& target, sf::RenderStates states) const override;

	/**
	 * \brief Sorts pipesets by distance from a given point.
	 * PipeSets are ordered by increasing distance.
	 * \param position Point from which the distance is calculated
	 * \return An ascending distance-sorted vector of Pipesets
	 */
	std::vector<const PipeSet*> sortedByDistancePipeSets(const sf::Vector2f& position) const;

	/**
	 * \brief Sorts pipesets by distance from a given point considering only those on the front of the point
	 * PipeSets are ordered by increasing distance.
	 * \param position Point from which the distance is calculated
	 * \return An ascending distance-sorted vector of Pipesets
	 */
	std::vector<const PipeSet*> sortedByDistancePipesetsInfrontOfPoint(const sf::Vector2f& position) const;

	/**
	 * \brief Checks if the bird and pipes are colliding
	 * (intersecting with each other)
	 * \param bird Reference to bird
	 */
	void checkCollision(Bird& bird) const;

	/**
	 * \brief Restarts the generator, starting generation again
	 * and deleting the other generated pipes
	 */
	void restart();

private:
	/**
	 * \brief Calculates random pipe offset using random number generator. 
	 * \return x and y offset values.
	 */
	[[nodiscard]] inline sf::Vector2f randomPipeOffset() const;

	/**
	 * \brief Retrieves the position of the last pipe added to the deque.
	 * \return Position (x, y) of the pipe.
	 */
	[[nodiscard]] inline sf::Vector2f lastPipeSetPosition() const;

	/**
	 * \brief Creates another pipe that is offset from the last pipe in the deque by the given offset.
	 * \param offset Offset from the last pipe in the deque
	 * \param pipeTextureId Pipe texture id
	 * \return Newly created pipe
	 */
	[[nodiscard]] std::unique_ptr<Pipe> createNextPipeWithOffset(const sf::Vector2f& offset, Textures_ID pipeTextureId = Textures_ID::Pipe_Green) const;

	/**
	 * \brief It generates both bottom and upper pipe.
	 */
	void generatePipe();

	/**
	 * \brief Deletes pipes outside of the window frame.
	 */
	void deleteFrontPipe();

	/**
	 * \brief Updates pipes' position on the screen.
	 * \param deltaTime the time that has passed since the game was last updated
	 */
	void updatePipesPosition(const sf::Time& deltaTime);

	/**
	 * \brief Determines if the pipe is outside the window area.
	 * \return True if the bird is out of sight. False otherwise.
	 */
	bool isFrontPipeSetOutOfSight() const;

	/**
	 * \brief Determines whether a pipe should be generated at a given time.
	 * \return Tru if the pipe is in window frame. False otherwise.
	 */
	bool isLastPipeSetInsideWindowFrame() const;

	/**
	 * \brief Updates the slider that sets the distance value between the upper and bottom pipes.
	 */
	void updateImGuiOffsetBetweenLowerAndUpperPipe();

	/**
	 * \brief Updates the movement pattern settings of newly created pipes
	 */
	void updateImGuiMovePattern();

	/**
	 * \brief Updates the movement pattern range of newly created pipes
	 */
	void updateImGuiMovePatternRange();

	/**
	 * \brief Updates the movement pattern speed of newly created pipes
	 */
	void updateImGuiMovePatternSpeed();

private:
	/** A manager that stores references to textures in the game */
	const TextureManager& mTextures;

	/** A manager that stores references to fonts in the game */
	const FontManager& mFonts;

	/**
	 * Specifies the minimum and maximum distance being an additional offset
	 * separating successive generated pipes in horizontal distance
	 */
	PipeOffset xCoordinate{ 85.f, 110.f };

	/**
	 * Specifies the additional vertical offset by which
	 * the pipe can move (up/down).
	 */
	PipeOffset yCoordinate{ 30.f, 0.f };

	/** Used to determine pipe clipping point */
	float mClippingPoint;

	/**	Distance between bottom and top pipe */
	float mOffsetBetweenPipes{55};

	/** An additional movement of newly created pipes. **/
	MovePattern mMovePattern;

	/** Produces high quality unsigned integer random numbers */
	static std::mt19937 engine;

	/** Random number generator, used in order to seed engine */
	static std::random_device rndDevice;

	/** Hold pipes that are currently being rendered on the screen */
	std::deque<PipeSet> mPipeSets;
};

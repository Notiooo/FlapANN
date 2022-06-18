#pragma once

#include "PipeMovePattern.h"
#include "nodes/NodeMoveable.h"
#include "resources/Resources.h"

/**
 * \brief Pipe is an object that blocks birds and collision with it leads to death of the bird.
 */
class Pipe final : public NodeMoveable
{
public:
	/**
	 * \brief The main constructor of the pipe.
	 * \param pipeTexture The texture the pipe should take.
	 * \param movePattern Additional movement pattern
	 */
	Pipe(const sf::Texture& pipeTexture, MovePattern movePattern = MovePattern());

	/**
	 * \brief Loads the required resources for this class.
	 * \param textureManager Texture storage manager.
	 */
	static void loadResources(TextureManager& textureManager);

	/**
	 * \brief Draws the pipe to the passed target.
	 * \param target where it should be drawn to
	 * \param states provides information about rendering process (transform, shader, blend mode).
	 */
	void drawThis(sf::RenderTarget& target, sf::RenderStates states) const override;

	/**
	 * \brief Updates the logic of the pipe. Including positions on the screen or
	 *		  the deletion or generation of the pipe.
	 * \param deltaTime the time that has passed since the game was last updated
	 */
	void updateThis(const sf::Time& deltaTime) override;

	/**
	 * \brief Returns the bounds of the pipe sprite.
	 * \return Width and height of the sprite.
	 */
	sf::FloatRect getPipeBounds() const;

	/**
	 * \brief Returns the speed at which the pipe is moving
	 * \return The speed at which the pipe is moving
	 */
	static float pipeSpeed();

private:

	/** Pipe graphics drawn on screen */
	sf::Sprite mPipe;

	/** The pattern by which the pipes move */
	MovePattern mCurrentMovePattern;

	/** The speed at which the pipe moves */
	inline static float mPipeSpeed = 40.f;
};

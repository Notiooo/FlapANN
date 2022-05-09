#pragma once
#include "Pipe.h"

/**
 * \brief A collection consisting of two pipes.
 * One at the top and one at the bottom.
 */
class PipeSet : public sf::Drawable
{
public:

    /**
	 * \brief Default constructor for a set of two pipes
	 * \param fontManager Font manager
	 * \param bottomPipe Pipe located at the bottom of the screen
	 * \param upperPipe Pipe located at the top of the screen
	 */
	PipeSet(const FontManager& fontManager, 
		std::unique_ptr<Pipe> bottomPipe, std::unique_ptr<Pipe> upperPipe);

    /**
	 * \brief Current position indicating the center between the two pipes
	 * \return Center position between two pipes
	 */
	sf::Vector2f position() const;

    /**
	 * \brief The bounds of a set of two pipes that contains
	 * the bounds of the upper and lower pipes together.
	 * \return The bounds of the two pipes
	 */
	sf::FloatRect bounds() const;

    /**
	 * \brief Returns free space between pipes
	 * \return Space between pipes
	 */
	float offsetBetweenPipes() const;

	/**
	 * \brief Updates the logic of the both pipes.
	 * \param deltaTime the time that has passed since the game was last updated
	 */
    void update(const sf::Time& deltaTime);

	/**
	 * \brief Draws the pipes to the passed target.
	 * \param target where it should be drawn to
	 * \param states provides information about rendering process (transform, shader, blend mode).
	 */
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    /**
	 * \brief Returns the pipe located at the bottom of the screen
	 * \return Reference to the bottom pipe
	 */
	Pipe& bottomPipe();

    /**
	 * \brief Returns the pipe located at the top of the screen
	 * \return Reference to the upper pipe
	 */
	Pipe& upperPipe();

    /**
	 * \brief Returns the pipe located at the bottom of the screen
	 * \return Const reference to the bottom pipe
	 */
	const Pipe& bottomPipe() const;

    /**
	 * \brief Returns the pipe located at the top of the screen
	 * \return Const reference to the upper pipe
	 */
	const Pipe& upperPipe() const;

private:
	std::unique_ptr<Pipe> mBottomPipe;
	std::unique_ptr<Pipe> mUpperPipe;
	sf::Text mOffsetBetweenPipesText;
	mutable bool mMakeRed = false;
};
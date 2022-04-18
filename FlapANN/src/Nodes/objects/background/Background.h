#pragma once
#include "nodes/NodeMoveable.h"
#include "resources/Resources.h"

class Background final : private NodeMoveable
{
public:
	/**
	 * \brief The main constructor of the background
	 * \param textureManager Texture storage manager
	 * \param scrollSpeed The speed at which the background moves
	 */
	Background(const TextureManager& textureManager, const float& scrollSpeed = 40.f);

	/**
	 * \brief Loads the required resources for this class
	 * \param textureManager Texture storage manager
	 */
	static void loadResources(TextureManager& textureManager);

	/**
	 * \brief Draws the background to the passed target.
	 * \param target where it should be drawn to
	 * \param states provides information about rendering process (transform, shader, blend mode)
	 */
	void drawThis(sf::RenderTarget& target, sf::RenderStates states) const override;

	/**
	 * \brief Updates the logic of the background. Including positions on the screen.
	 * \param deltaTime the time that has passed since the game was last updated
	 *
	 * Works analogues to the drawThis(), updates all things related to itself
	 */
	void updateThis(const sf::Time& deltaTime) override;

	/**
	 * \brief Stops the background from moving
	 */
	void stop();

	/**
	 * \brief Makes the background start to move
	 */
	void start();

	/**
	 * \brief Sets the background moving speed
	 * \param scrollSpeed Speed of background movement
	 */
	void setScrollSpeed(const float& scrollSpeed);

private:
	/**
	 * \brief Checks if half of the background is already at the starting background position.
	 * \return True if half the background is at the start position, false otherwise.
	 */
	bool isMiddleOfBackgroundAtStart() const;

	/**
	 * \brief Sets the background to its original position
	 */
	void repositionBackground();

	/**
	 * \brief Extends a texture by the specified multiplier
	 * \param widthMultiplier Multiply how long the texture will be
	 */
	void makeTextureLonger(const float& widthMultiplier);

private:
	float mScrollSpeed;
	sf::Sprite mBackground;
};
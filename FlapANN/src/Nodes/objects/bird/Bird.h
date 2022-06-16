#pragma once
#include "nodes/NodeMoveable.h"
#include "resources/Resources.h"

class Bird final : public NodeMoveable
{
public:
	/**
	 * \brief The main constructor of the bird
	 * \param birdTexture The texture the bird should take
	 */
	Bird(const sf::Texture& birdTexture);

	/**
	 * \brief Makes the bird "hop/flap" upwards
	 */
	void flap();

	/**
	 * \brief Kills the bird, meaning it can no longer flap.
	 */
	void kill();
    bool isDead() const;

    /**
	 * \brief Loads the required resources for this class
	 * \param textureManager Texture storage manager
	 */
	static void loadResources(TextureManager& textureManager);

	/**
	 * \brief It takes input (event) from the user and interprets it
	 * \param event user input
	 */
	void handleThisEvents(const sf::Event& event) override;

	/**
	 * \brief Draws the bird to the passed target.
	 * \param target where it should be drawn to
	 * \param states provides information about rendering process (transform, shader, blend mode)
	 */
	void drawThis(sf::RenderTarget& target, sf::RenderStates states) const override;

	/**
	 * \brief Updates the logic of the bird. Including positions on the screen, or its rotations.
	 * \param deltaTime the time that has passed since the game was last updated
	 *
	 * Works analogues to the drawThis(), updates all things related to itself
	 */
	void updateThis(const sf::Time& deltaTime) override;

	/**
	 * \brief Function that returns the bounds of the bird
	 * sprite. It is then used in functions that checks
	 * colision with other objects.
	 * \return Bounds of the sprite
	 */
	sf::FloatRect getBirdBounds() const;

	float birdScore() const;

private:
	/**
	 * \brief Determines if the bird is falling at this point.
	 * \param fallingThreshold Offsetting to allow for fall reporting later.
	 * \return True if the bird falls. False otherwise.
	 */
	bool isBirdFalling(const float& fallingThreshold) const;

	/**
	 * \brief Determines if the bird is rising (flapping) at this point.
	 * \return True if the bird is raising. False otherwise.
	 */
	bool isBirdRaising() const;


	/**
	 * \brief Calculates the current rotation that the bird should make.
	 * \param fallingThreshold Offsetting to allow for fall reporting later.
	 * \return Rotation in degrees that the bird should make
	 */
	float calculateRotationChange(const float& fallingThreshold) const;


	/**
	 * \brief Updates the rotation of the bird
	 * \param deltaTime the time that has passed since the game was last updated
	 */
	void updateRotation(const sf::Time& deltaTime);
	void updateScore(const sf::Time& deltaTime);

private:
	float mBirdScore = 0;
	float mJumpStrength = 185.f;
	bool mIsKilled = false;
	sf::Sprite mBird;
};

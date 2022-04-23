#pragma once

#include "nodes/NodeMoveable.h"
#include "resources/Resources.h"

class Pipe final : public NodeMoveable
{
public:
	/**
	 * \brief The main constructor of the pipe.
	 * \param pipeTexture The texture the pipe should take.
	 */
	explicit Pipe(const sf::Texture& pipeTexture);

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
	 * \brief Returns the bounds of the pipe sprite.
	 * \return Width and height of the sprite.
	 */
	sf::FloatRect getPipeBounds() const;

private:
	sf::Sprite mPipe;
};

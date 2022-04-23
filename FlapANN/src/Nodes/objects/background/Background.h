#pragma once
#include "nodes/NodeScrollable.h"
#include "resources/Resources.h"

class Background final : public NodeScrollable
{
public:
	/**
	 * \brief The main constructor of the background
	 * \param textureManager Texture storage manager
	 * \param scrollSpeed The speed at which the background moves
	 */
	Background(const TextureManager& textureManager, const float& scrollSpeed = 10.f);

	/**
	 * \brief Loads the required resources for this class
	 * \param textureManager Texture storage manager
	 */
	static void loadResources(TextureManager& textureManager);
};
#pragma once
#include "nodes/NodeScrollable.h"
#include "resources/Resources.h"

class Ground final : public NodeScrollable
{
public:
	/**
	 * \brief The main constructor of the ground
	 * \param textureManager Texture storage manager
	 * \param scrollSpeed The speed at which the ground moves
	 */
	Ground(const TextureManager& textureManager, const float& scrollSpeed = 40.f);

	/**
	 * \brief Loads the required resources for this class
	 * \param textureManager Texture storage manager
	 */
	static void loadResources(TextureManager& textureManager);
};
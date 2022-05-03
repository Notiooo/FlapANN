#include "pch.h"
#include "Ground.h"


Ground::Ground(const TextureManager& textureManager, const float& scrollSpeed)
	: NodeScrollable(textureManager.getResourceReference(Textures_ID::Ground), scrollSpeed)
{
	setOrigin(0, mBackground.getTexture()->getSize().y);
}

void Ground::loadResources(TextureManager& textureManager)
{
	textureManager.storeResource(Textures_ID::Ground, "resources/textures/background/ground.png");
	textureManager.getResourceReference(Textures_ID::Ground).setRepeated(true);
}
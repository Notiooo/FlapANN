#include "pch.h"
#include "Background.h"


Background::Background(const TextureManager& textureManager, const float& scrollSpeed)
	: NodeScrollable(textureManager.getResourceReference(Textures_ID::Background_Day), scrollSpeed)
{

}

void Background::loadResources(TextureManager& textureManager)
{
	textureManager.storeResource(Textures_ID::Background_Day, "resources/textures/background/background.png");
	textureManager.getResourceReference(Textures_ID::Background_Day).setRepeated(true);
}
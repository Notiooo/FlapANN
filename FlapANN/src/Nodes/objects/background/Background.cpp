#include "pch.h"
#include "Background.h"


Background::Background(const TextureManager& textureManager, const float& scrollSpeed)
	: mBackground(textureManager.getResourceReference(Textures_ID::Background_Day))
	, mScrollSpeed(scrollSpeed)
{
	setVelocity({ -mScrollSpeed, 0.f });
	mBackground.setOrigin(0, 0);
	makeTextureLonger(2.f);
}

void Background::makeTextureLonger(const float& widthMultiplier)
{
	const auto& textureRect = mBackground.getTextureRect();
	mBackground.setTextureRect(sf::IntRect(0, 0, 
		static_cast<int>(static_cast<float>(textureRect.width) * widthMultiplier), textureRect.height));
}

void Background::loadResources(TextureManager& textureManager)
{
	textureManager.storeResource(Textures_ID::Background_Day, "resources/textures/background/background.png");
	textureManager.getResourceReference(Textures_ID::Background_Day).setRepeated(true);
}

void Background::drawThis(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mBackground, states);
}

bool Background::isMiddleOfBackgroundAtStart() const
{
	const static sf::Vector2u textureSize = mBackground.getTexture()->getSize();
	return getPosition().x < -static_cast<float>(textureSize.x);
}

void Background::repositionBackground()
{
	setPosition(0,0);
}

void Background::updateThis(const sf::Time& deltaTime)
{
	NodeMoveable::updateThis(deltaTime);

	if(isMiddleOfBackgroundAtStart())
	{
		repositionBackground();
	}
}

void Background::stop()
{
	setVelocity({ 0, 0 });
}

void Background::start()
{
	setVelocity({ -mScrollSpeed, 0.f });
}

void Background::setScrollSpeed(const float& scrollSpeed)
{
	setVelocity({ -scrollSpeed, 0.f });
}

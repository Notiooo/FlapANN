#include "pch.h"
#include "NodeScrollable.h"

#include "objects/background/Background.h"


NodeScrollable::NodeScrollable(const sf::Texture& texture, const float& scrollSpeed)
	: mScrollSpeed(scrollSpeed)
	, mBackground(texture)
{
	setVelocity({ -mScrollSpeed, 0.f });
	mBackground.setOrigin(0, 0);
	makeTextureLonger(3.f);
}

void NodeScrollable::makeTextureLonger(const float& widthMultiplier)
{
	const auto& textureRect = mBackground.getTextureRect();
	mBackground.setTextureRect(sf::IntRect(0, 0, 
		static_cast<int>(static_cast<float>(textureRect.width) * widthMultiplier), textureRect.height));
}

void NodeScrollable::drawThis(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mBackground, states);
}

bool NodeScrollable::isMiddleOfBackgroundAtStart() const
{
	const static sf::Vector2u textureSize = mBackground.getTexture()->getSize();
	return getPosition().x < -static_cast<float>(textureSize.x);
}

void NodeScrollable::repositionBackground()
{
	setPosition(0,getPosition().y);
}

void NodeScrollable::updateThis(const sf::Time& deltaTime)
{
	NodeMoveable::updateThis(deltaTime);

	if(isMiddleOfBackgroundAtStart())
	{
		repositionBackground();
	}
}

void NodeScrollable::stop()
{
	setVelocity({ 0, 0 });
}

void NodeScrollable::start()
{
	setVelocity({ -mScrollSpeed, 0.f });
}

void NodeScrollable::setScrollSpeed(const float& scrollSpeed)
{
	setVelocity({ -scrollSpeed, 0.f });
}

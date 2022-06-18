#include "pch.h"
#include "PipeSet.h"

PipeSet::PipeSet(const FontManager& fontManager, std::unique_ptr<Pipe> bottomPipe, std::unique_ptr<Pipe> upperPipe)
    : mBottomPipe(std::move(bottomPipe))
    , mUpperPipe(std::move(upperPipe))
    , mOffsetBetweenPipesText(std::to_string(static_cast<int>(offsetBetweenPipes())), 
		fontManager.getResourceReference(Fonts_ID::ArialNarrow))
{
	mOffsetBetweenPipesText.setCharacterSize(12);
	mOffsetBetweenPipesText.setOutlineThickness(0.25f);
	mOffsetBetweenPipesText.setOutlineColor(sf::Color::Black);
	mOffsetBetweenPipesText.setOrigin(mOffsetBetweenPipesText.getLocalBounds().width / 2.f, 
		                            mOffsetBetweenPipesText.getLocalBounds().height / 2.f);
}

sf::Vector2f PipeSet::position() const
{
	auto xPosition = mBottomPipe->getPosition().x;
	auto yPosition = mUpperPipe->getPosition().y + (offsetBetweenPipes() / 2.f);
	return { xPosition, yPosition };
}

sf::FloatRect PipeSet::bounds() const
{
	auto bottomPipeBounds = mBottomPipe->getPipeBounds();
	auto upperPipeBounds = mUpperPipe->getPipeBounds();
	return {
        ((bottomPipeBounds.left < upperPipeBounds.left) ? bottomPipeBounds.left :  upperPipeBounds.left),
		((bottomPipeBounds.top < upperPipeBounds.top) ? bottomPipeBounds.top :  upperPipeBounds.top),
		((bottomPipeBounds.width > upperPipeBounds.width) ? bottomPipeBounds.width :  upperPipeBounds.width),
		((bottomPipeBounds.height > upperPipeBounds.height) ? bottomPipeBounds.height :  upperPipeBounds.height)
    };
}

float PipeSet::offsetBetweenPipes() const
{
	return mBottomPipe->getPosition().y - mUpperPipe->getPosition().y;
}

void PipeSet::update(const sf::Time& deltaTime)
{
	mBottomPipe->update(deltaTime);
	mUpperPipe->update(deltaTime);
	mOffsetBetweenPipesText.setPosition({ position().x + 10.f, position().y });
}

void PipeSet::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(*mBottomPipe, states);
	target.draw(*mUpperPipe, states);
	target.draw(mOffsetBetweenPipesText, states);

	sf::Vertex line[] =
	{
		sf::Vertex(sf::Vector2f(bottomPipe().getPosition().x, bottomPipe().getPosition().y)),
		sf::Vertex(sf::Vector2f(upperPipe().getPosition().x,  upperPipe().getPosition().y)),
	};
	target.draw(line, 2, sf::Lines, states);
}

Pipe& PipeSet::bottomPipe()
{
	return *mBottomPipe;
}

Pipe& PipeSet::upperPipe()
{
	return *mUpperPipe;
}

const Pipe& PipeSet::bottomPipe() const
{
	return *mBottomPipe;
}

const Pipe& PipeSet::upperPipe() const
{
	return *mUpperPipe;
}

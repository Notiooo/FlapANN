#include "pch.h"
#include "PipesGenerator.h"
#include <random>
#include <imgui/imgui.h>

std::random_device PipesGenerator::rndDevice;
std::mt19937 PipesGenerator::engine(rndDevice());

PipesGenerator::PipesGenerator(const TextureManager& textures, const sf::Vector2i& screenSize):
	mTextures(textures),
	mClippingPoint(screenSize.x)
{
	yCoordinate.maxPipeOffset = screenSize.y / 2;
}

sf::Vector2f PipesGenerator::randomPipeOffset() const
{
	const std::uniform_real_distribution randomizedXPosition(xCoordinate.minPipeOffset, xCoordinate.maxPipeOffset);
	const std::uniform_real_distribution randomizedYPosition(yCoordinate.minPipeOffset, yCoordinate.maxPipeOffset);

	auto pipeXOffset = randomizedXPosition(engine);
	auto upperPipeHeight = randomizedYPosition(engine);

	return {pipeXOffset, upperPipeHeight};
}

sf::Vector2f PipesGenerator::lastPipePosition() const
{
	return mPipes.empty() ? sf::Vector2f{0.f, 0.f} : mPipes.back()->getPosition();
}

std::unique_ptr<Pipe> PipesGenerator::createNextPipeWithOffset(const sf::Vector2f& offset, Textures_ID pipeTextureId) const
{
	const auto& pipeTexture = mTextures.getResourceReference(pipeTextureId);
	auto pipe = std::make_unique<Pipe>(pipeTexture, mMovePattern);
	pipe->setPosition({lastPipePosition().x + offset.x, offset.y});
	pipe->setOrigin(static_cast<float>(pipeTexture.getSize().x) / 2.f, 0);

	return pipe;
}

bool PipesGenerator::isFrontPipeOutOfSight() const
{
	const auto& pipe = mPipes.front();
	return !mPipes.empty() && pipe->getPosition().x + pipe->getPipeBounds().width < 0.f;
}

bool PipesGenerator::isLastPipeInsideWindowFrame() const
{
	return mPipes.empty() || mPipes.back()->getPosition().x < mClippingPoint;
}

void PipesGenerator::generatePipe()
{
	const sf::Vector2f& offset = randomPipeOffset();

	auto bottomPipe = createNextPipeWithOffset({ offset.x, offset.y + mOffsetBetweenPipes / 2.f });
	auto upperPipe = createNextPipeWithOffset({ offset.x, offset.y - mOffsetBetweenPipes / 2.f });;
	upperPipe->setRotation(180);

	mPipes.emplace_back(std::move(bottomPipe));
	mPipes.emplace_back(std::move(upperPipe));
}

void PipesGenerator::deleteFrontPipe()
{
	mPipes.pop_front();
}

void PipesGenerator::updatePipesPosition(const sf::Time& deltaTime) const
{
	for (const auto& pipe : mPipes)
	{
		pipe->update(deltaTime);
	}
}

void PipesGenerator::updateThis(const sf::Time& deltaTime)
{
	if (isLastPipeInsideWindowFrame())
	{
		generatePipe();
	}
	if (isFrontPipeOutOfSight())
	{
		deleteFrontPipe();
	}
	updatePipesPosition(deltaTime);
}

void PipesGenerator::updateImGuiOffsetBetweenLowerAndUpperPipe()
{
	const static auto& sliderText = "Offset between upper and lower pipe";
	const static auto& textSize = ImGui::CalcTextSize(sliderText);

	ImGui::PushItemWidth(-textSize.x);
	ImGui::SliderFloat(sliderText, &mOffsetBetweenPipes, 0.f, 100.f);
}

void PipesGenerator::updateImGuiMovePattern()
{
	ImGui::SetNextItemOpen(true, ImGuiCond_Once);
	if (ImGui::TreeNode("Pattern"))
	{
		const auto selected = static_cast<int>(mMovePattern.pattern());
		for (auto patternIndex = static_cast<int>(MovePattern::Pattern::None);
		     patternIndex < static_cast<int>(MovePattern::Pattern::Last); ++patternIndex)
		{
			auto pattern = static_cast<MovePattern::Pattern>(patternIndex);
			if (ImGui::Selectable(toString(pattern).c_str(), selected == patternIndex))
			{
				mMovePattern.applyPattern(pattern);
			}
		}
		ImGui::TreePop();
	}
}

void PipesGenerator::updateImGuiMovePatternRange()
{
	const static auto& sliderText = "Range of the movement pattern";
	const static auto& textSize = ImGui::CalcTextSize(sliderText);
	auto movePatternRange = mMovePattern.patternRange();

	ImGui::PushItemWidth(-textSize.x);
	if(ImGui::SliderFloat(sliderText, &movePatternRange, 0.f, 2.f))
	{
		mMovePattern.patternRange(movePatternRange);
	}
}

void PipesGenerator::updateImGuiMovePatternSpeed()
{
	const static auto& sliderText = "Speed of the movement pattern";
	const static auto& textSize = ImGui::CalcTextSize(sliderText);
	auto movePatternSpeed = mMovePattern.patternSpeed();

	ImGui::PushItemWidth(-textSize.x);
	if(ImGui::SliderFloat(sliderText, &movePatternSpeed, 0.f, 8.f))
	{
		mMovePattern.patternSpeed(movePatternSpeed);
	}
}

void PipesGenerator::updateImGuiThis()
{
	if(ImGui::CollapsingHeader("PipeGenerator"))
	{
		updateImGuiOffsetBetweenLowerAndUpperPipe();

		ImGui::SetNextItemOpen(true, ImGuiCond_Once);
		if (ImGui::CollapsingHeader("Move pattern settings"))
		{
			updateImGuiMovePattern();
			updateImGuiMovePatternRange();
			updateImGuiMovePatternSpeed();
		}
	}
}

void PipesGenerator::drawThis(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const auto& pipe : mPipes)
	{
		pipe->draw(target, states);
	}
}

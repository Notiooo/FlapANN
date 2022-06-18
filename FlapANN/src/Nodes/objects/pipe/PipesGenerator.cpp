#include "pch.h"
#include "PipesGenerator.h"
#include <random>
#include <imgui/imgui.h>

std::random_device PipesGenerator::rndDevice;
std::mt19937 PipesGenerator::engine(rndDevice());

PipesGenerator::PipesGenerator(const TextureManager& textures, const FontManager& fonts, const sf::Vector2u& screenSize):
	mTextures(textures),
	mFonts(fonts),
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

sf::Vector2f PipesGenerator::lastPipeSetPosition() const
{
	return mPipeSets.empty() ? sf::Vector2f{mClippingPoint, 0.f} : mPipeSets.back().position();
}

std::unique_ptr<Pipe> PipesGenerator::createNextPipeWithOffset(const sf::Vector2f& offset, Textures_ID pipeTextureId) const
{
	const auto& pipeTexture = mTextures.getResourceReference(pipeTextureId);
	auto pipe = std::make_unique<Pipe>(pipeTexture, mMovePattern);
	pipe->setPosition({lastPipeSetPosition().x + offset.x, offset.y});
	pipe->setOrigin(static_cast<float>(pipeTexture.getSize().x) / 2.f, 0);

	return pipe;
}

bool PipesGenerator::isFrontPipeSetOutOfSight() const
{
	const auto& pipe = mPipeSets.front();
	return !mPipeSets.empty() && pipe.position().x + pipe.bounds().width < 0.f;
}

bool PipesGenerator::isLastPipeSetInsideWindowFrame() const
{
	return mPipeSets.empty() || mPipeSets.back().position().x < mClippingPoint;
}

void PipesGenerator::generatePipe()
{
	const sf::Vector2f& offset = randomPipeOffset();

	auto bottomPipe = createNextPipeWithOffset({ offset.x, offset.y + mOffsetBetweenPipes / 2.f });
	auto upperPipe = createNextPipeWithOffset({ offset.x, offset.y - mOffsetBetweenPipes / 2.f });
	upperPipe->setRotation(180);

	mPipeSets.emplace_back(PipeSet{ mFonts, std::move(bottomPipe), std::move(upperPipe) });
}

void PipesGenerator::deleteFrontPipe()
{
	mPipeSets.pop_front();
}

void PipesGenerator::updatePipesPosition(const sf::Time& deltaTime)
{
	for (auto& pipeSet : mPipeSets)
	{
		pipeSet.update(deltaTime);
	}
}

void PipesGenerator::updateThis(const sf::Time& deltaTime)
{
	if (isLastPipeSetInsideWindowFrame())
	{
		generatePipe();
	}
	while(isFrontPipeSetOutOfSight())
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
	if(ImGui::SliderFloat(sliderText, &movePatternRange, 0.f, 1.5f))
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
	if(ImGui::SliderFloat(sliderText, &movePatternSpeed, 0.f, 5.f))
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
	for (const auto& pipe : mPipeSets)
	{
		pipe.draw(target, states);
	}
}

std::vector<const PipeSet*> PipesGenerator::sortedByDistancePipeSets(const sf::Vector2f& position) const
{
	auto calculateHorizontalDistance = [&position](const sf::Vector2f& newPos)
	{
		return std::sqrtf(std::powf(position.x - newPos.x, 2));
	};

	std::vector<const PipeSet*> pipeSetPtrs;
	for(const auto& pipeSet : mPipeSets)
	{
		pipeSetPtrs.push_back(&pipeSet);
	}

	std::sort(pipeSetPtrs.begin(), pipeSetPtrs.end(), 
		[&calculateHorizontalDistance](const PipeSet* a, const PipeSet* b)
	{
		return calculateHorizontalDistance(a->position()) < calculateHorizontalDistance(b->position());
	});

	return pipeSetPtrs;
}

std::vector<const PipeSet*> PipesGenerator::sortedByDistancePipesetsInfrontOfPoint(const sf::Vector2f& position) const
{
	static const auto& pipeWidth = static_cast<float>(mTextures.getResourceReference(Textures_ID::Pipe_Green).getSize().x);
	auto neartestPipes = sortedByDistancePipeSets(position);
	neartestPipes.erase(
		std::remove_if(neartestPipes.begin(), neartestPipes.end(),
			[&position](const PipeSet* pipe) { return position.x > pipe->position().x + pipeWidth / 1.8f;  }),
	neartestPipes.end());
	return neartestPipes;
}

void PipesGenerator::checkCollision(Bird& bird) const
{
	for (const auto& pipeSet : mPipeSets)
	{
		for (const auto& pipe : {std::ref(pipeSet.bottomPipe()), std::ref(pipeSet.upperPipe())})
		{
			if (pipe.get().getPipeBounds().intersects(bird.getBirdBounds()))
			{
				bird.kill();

				auto birdVelocity = (bird.velocity().y < 0) ? 0 : bird.velocity().y;
				bird.setVelocity({-Pipe::pipeSpeed(), birdVelocity});
			}
		}
	}
}

void PipesGenerator::restart()
{
	mPipeSets.clear();
}

#include "pch.h"
#include "PipeMovePattern.h"
#include "PipeMovePattern.h"


MovePattern::MovePattern()
	: mCurrentMovePattern(Pattern::None)
	, mMovePatternSpeed(0)
	, mMovePatternIteration(0)
	, mMovePatternRange(0)
{
}

void MovePattern::applyPattern(const Pattern& pattern)
{
	mCurrentMovePattern = pattern;
	mMovePatternIteration = 0;
}

MovePattern::Pattern MovePattern::pattern() const
{
	return mCurrentMovePattern;
}

void MovePattern::patternSpeed(const float& speed)
{
	mMovePatternSpeed = speed;
}

float MovePattern::patternSpeed()
{
	return mMovePatternSpeed;
}

void MovePattern::patternRange(const float& range)
{
	mMovePatternRange = range;
}

float MovePattern::patternRange()
{
	return mMovePatternRange;
}

sf::Vector2f MovePattern::positionDelta(const sf::Time& deltaTime)
{
	mMovePatternIteration += deltaTime.asSeconds() * mMovePatternSpeed;
	auto moveDeltaSin = std::sinf(mMovePatternIteration) * mMovePatternRange;

	switch (mCurrentMovePattern)
	{
		case Pattern::InCircle:
		{
			auto moveDeltaCos = std::cosf(mMovePatternIteration) * mMovePatternRange;
			return { moveDeltaSin, moveDeltaCos };
		}
		case Pattern::UpDown:
		{
			return { 0, moveDeltaSin };
		}
		case Pattern::LeftRight:
		{
			return { moveDeltaSin, 0 };
		}
		default: return {  0, 0  };
	}
}

std::string toString(const MovePattern::Pattern& pattern)
{
	switch(pattern)
	{
	case MovePattern::Pattern::None:        return "None";
	case MovePattern::Pattern::InCircle:    return "In circle";
	case MovePattern::Pattern::UpDown:      return "Up and Down";
	case MovePattern::Pattern::LeftRight:   return "Left and Right";
	default:                                return "Not found";
	}
}

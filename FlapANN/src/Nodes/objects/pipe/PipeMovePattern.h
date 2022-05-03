#pragma once

/**
 * \brief MovePattern calculates and returns the offsets
 * by which the object should be additionally moved in
 * order to move around the given pattern.
 */
class MovePattern final
{
public:
	MovePattern();

	/**
	 * \brief The pattern with which the pipe additionally moves 
	 */
	enum class Pattern
	{
		None,
		InCircle,
		UpDown,
		LeftRight,

		Last,
	};

	/**
	 * \brief Sets a new pattern
	 * \param pattern new pattern
	 */
	void applyPattern(const Pattern& pattern);

	/**
	 * \brief Returns the current pattern
	 * \return Currently selected pattern
	 */
	[[nodiscard]] Pattern pattern() const;

	/**
	 * \brief Sets a new speed for the current pattern
	 * \param speed a new speed
	 */
	void patternSpeed(const float& speed);

	/**
	 * \brief Returns the speed of the current pattern
	 * \return Speed of current pattern
	 */
	float patternSpeed();

	/**
	 * \brief Sets a new range for the current pattern
	 * \param range a new range
	 */
	void patternRange(const float& range);

	/**
	 * \brief Returns the range of the current pattern
	 * \return Range of current pattern
	 */
	float patternRange();

	/**
	 * \brief Position by which the object should be moved in addition
	 * \param deltaTime the time that has passed since the game was last updated
	 * \return Position by which the object should be moved in addition
	 */
	sf::Vector2f positionDelta(const sf::Time& deltaTime);

private:
	/** Currently selected pattern */
	Pattern mCurrentMovePattern;

	/** Speed of the currently selected pattern */
	float mMovePatternSpeed;

	/**
	 * Iteration allows to specify the time at which a particular
	 * moment of the pattern is displayed
	 */
	float mMovePatternIteration;

	/** Range of the currently selected pattern */
	float mMovePatternRange;
};

/**
 * \brief Converts pattern to text
 * \param pattern Move pattern
 * \return Name of pattern
 */
std::string toString(const MovePattern::Pattern& pattern);

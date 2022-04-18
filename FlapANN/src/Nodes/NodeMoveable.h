#ifndef ENTITY_H_
#define ENTITY_H_

#include "NodeScene.h"

/**
* \brief The kind of scene that contains movement
* A specific type of scene that contains physical elements such as acceleration, speed.
*/
class NodeMoveable : public NodeScene
{
	public:
		/** 
		* Set velocity of this object
		* @param velocity Vector with velocity applied to this object
		*/
		void setVelocity(sf::Vector2f velocity);

		/** 
		* Set velocity of this object
		* @param velocityX Velocity on the x-axis
		* @param velocityY Velocity on the y-axis
		*/
		template <typename T>
		void setVelocity(T velocityX, T velocityY);


		/**
		* Returns current velocity of this object
		* @return Velocity of this object 
		*/
		sf::Vector2f velocity() const;

		/** 
		* Adds velocity to current velocity
		* @param velocity Velocity we want to add to this object
		*/
		void accelerate(sf::Vector2f velocity);

	public:
		/**
		 * \brief Updates the logic of this node only
		 * \param deltaTime the time that has passed since the game was last updated
		 */
		void updateThis(const sf::Time& deltaTime) override;

	private:
		/** Velocity of the object at this time */
		sf::Vector2f mVelocity;
};

template <typename T>
void NodeMoveable::setVelocity(T velocityX, T velocityY)
{
	mVelocity = { velocityX, velocityY };
}
#endif

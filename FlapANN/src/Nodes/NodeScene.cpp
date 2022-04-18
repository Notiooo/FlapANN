#include "pch.h"
#include "Nodes/NodeScene.h"

#include <algorithm>
#include <cassert>
#include <functional>

NodeScene::NodeScene() :
	mPinnedNodes(),
	mParent(nullptr)
{
}

void NodeScene::pinNode(Node node)
{
	// Now this Scene is mParent of given node
	node->mParent = this;

	// So lets add it into mPinnedNodes of this Scene
	mPinnedNodes.push_back(std::move(node));

}

NodeScene::Node NodeScene::unpinNode(const NodeScene& node_scene)
{
	// The address of given NodeScene have to match one of the addresses inside our vector of mPinnedNodes
	auto foundNode = std::find_if(mPinnedNodes.begin(), mPinnedNodes.end(), [&node_scene](Node& pinned_node) -> bool { return pinned_node.get() == &node_scene; });

	// Make sure that we found such a Node to unpin
	assert(foundNode != mPinnedNodes.end());

	// Now as we found the given NodeScene we must unpin it from our vector
	// First take the ownership from the vector
	auto stolenNode = std::move(*foundNode);

	// std::move makes sure it is in valid state to run destructor, so we delete it
	mPinnedNodes.erase(foundNode);
	
	// Right now the stolen_node is not part of this Scene anymore.
	// We may remove its "parentness"
	stolenNode->mParent = nullptr;

	return stolenNode;
}

sf::Vector2f NodeScene::absolutePosition() const
{
	// To do this we have to add all transforms till the top of the hierarchy.
	sf::Transform transform; //sf::Transform::Identity;
	for (const auto* node = this; node != nullptr; node = node->mParent)
		transform = node->getTransform() * transform;
	
	// By applying this transform on empty Vector2f we move it to the desired position
	return transform * sf::Vector2f();
}

void NodeScene::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// We want to draw this object in relation to the mParent
	// states.transform stores information about mParent scene render position
	// by multiplying it by our getTransform() position we get position related
	// to our mParent

	states.transform *= getTransform(); // Absolute world transform is result of this operation

	// As our object is in the right position, we can finally draw it
	drawThis(target, states);

	// Right after we did this, we can forward drawing to the nodes lower in the hierarchy.
	// This way we will draw all nodes to the screen starting from the root.
	// Also it will help us to maintain the order of which should be in front or in the back of the screen
	for (const auto& pinnedNode : mPinnedNodes)
		pinnedNode->draw(target, states);
}

void NodeScene::drawThis(sf::RenderTarget& target, sf::RenderStates states) const
{
	// Nothing here
}

void NodeScene::update(const sf::Time& deltaTime)
{
	// We starting by updating this object
	updateThis(deltaTime);

	// And then we update all mPinnedNodes -- which updates all nodes pinned to them
	for (auto& pinnedNode : mPinnedNodes)
		pinnedNode->update(deltaTime);
}

void NodeScene::updateThis(const sf::Time& deltaTime)
{
	// Nothing here
}

void NodeScene::handleEvents(const sf::Event& event)
{
	// Handle event in this node
	handleThisEvents(event);

	// And next pass the event to all pinned nodes
	for (auto& pinnedNode : mPinnedNodes)
		pinnedNode->handleEvents(event);
}

void NodeScene::handleThisEvents(const sf::Event& event)
{
}

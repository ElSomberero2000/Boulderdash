// Project Includes
#include "Box.h"
#include "Framework/AssetManager.h"
#include "Level.h"
#include "Dirt.h"
#include "Gem.h"

Box::Box()
	: GridObject()
	, m_pushSound()
	, m_stored(false)
	, m_timer()
	, fallTime(0.75)
	, m_falling(true)
	, m_sliding(false)
{
	m_sprite.setTexture(AssetManager::GetTexture("graphics/box.png"));
	// Set collision state
	m_blocksMovement = true;
	m_pushSound.setBuffer(AssetManager::GetSoundBuffer("audio/push.wav"));

}

bool Box::AttemptFall(sf::Vector2i _direction)
{
	// Attempt to move in the given direction

	// Get current position
	// Calculate target position
	sf::Vector2i targetPos = m_gridPosition + _direction;

	// Check if the space is empty

	// Get list of objects in our target position
	std::vector<GridObject*> targetCellContents = m_level->GetObjectAt(targetPos);


	// Check if any of those objects block movement
	bool blocked = false;
	GridObject* blocker = nullptr;
	for (int i = 0; i < targetCellContents.size(); ++i)
	{
		if (targetCellContents[i]->GetBlocksMovement() == true)
		{
			blocked = true;
			blocker = targetCellContents[i];
		}
	}

	// If empty, move there
	if (blocked == false)
	{
		return m_level->MoveObjectTo(this, targetPos);
	}
	else
	{
		// We were blocked!
		// Can we push the thing blocking us?
		// Do a dynamic cast to a box to see if we can push it
		Box* pushableBox = dynamic_cast<Box*>(blocker);
		Player* livingPlayer = dynamic_cast<Player*>(blocker);
		Gem* onDiamond = dynamic_cast<Gem*>(blocker);
		// If so (the blocker is a box (not nullptr))

		// If so (the blocker is the player (not nullptr))
		// If box attempts to move into player occupied space reload level
		if (livingPlayer != nullptr)
		{
			m_level->ReloadLevel();
		}
		// If box attempts to move into box occupied space slide
		if (pushableBox != nullptr)
		{
			// Try to slide right, if not availible try left
			if (!AttemptSlide(sf::Vector2i(1, 0)))
			{
				AttemptSlide(sf::Vector2i(-1, 0));
			}
			
		}
		// If box attempts to move into diamond occupied space slide
		if (onDiamond != nullptr)
		{
			// Try to slide right, if not availible try left
			if (!AttemptSlide(sf::Vector2i(1, 0)))
			{
				AttemptSlide(sf::Vector2i(-1, 0));
			}
		}
	}

	// If movement is blocked, do nothing, return false
	// Default
	return false;
}

bool Box::AttemptSlide(sf::Vector2i _direction)
{
	// Attempt to move in the given direction

	// Get current position
	// Calculate target position
	sf::Vector2i targetPos = m_gridPosition + _direction;

	// Check if the space is empty

	// Get list of objects in our target position
	std::vector<GridObject*> targetCellContents = m_level->GetObjectAt(targetPos);


	// Check if any of those objects block movement
	bool blocked = false;
	GridObject* blocker = nullptr;
	for (int i = 0; i < targetCellContents.size(); ++i)
	{
		if (targetCellContents[i]->GetBlocksMovement() == true)
		{
			blocked = true;
			blocker = targetCellContents[i];
		}
	}

	// If empty, move there
	if (blocked == false)
	{
		return m_level->MoveObjectTo(this, targetPos);
	}
	else
	{
		// We were blocked!
		// Can we push the thing blocking us?
		// Do a dynamic cast to a box to see if we can push it
		Box* pushableBox = dynamic_cast<Box*>(blocker);
		Player* livingPlayer = dynamic_cast<Player*>(blocker);
		// If so (the blocker is a box (not nullptr))

	}

	// If movement is blocked, do nothing, return false
	// Default
	return false;
}

void Box::Update(sf::Time _frameTime)
{
	// Increase timer gradually respective of the frametime i.e. every update timer = timer + 0.02
	m_timer += _frameTime.asSeconds();
	if (m_timer > fallTime)
	{
	
			AttemptFall(sf::Vector2i(0, 1));
		// Reset timer
		m_timer = 0;
	}
}

bool Box::AttemptPush(sf::Vector2i _direction)
{
	// Attempt to move the box in the given direction

	// Get current position
	// Calculate target position
	sf::Vector2i targetPos = m_gridPosition + _direction;

	// Check if the space is empty

	// Get list of objects in our target position
	std::vector<GridObject*> targetCellContents = m_level->GetObjectAt(targetPos);

	// Check if any of those objects block movement
	bool blocked = false;
	for (int i = 0; i < targetCellContents.size(); ++i)
	{
		if (targetCellContents[i]->GetBlocksMovement() == true)
		{
			blocked = true;
		}
	}

	// If empty, move there
	if (blocked == false)
	{
		m_pushSound.play();

		return m_level->MoveObjectTo(this, targetPos);
	}

	// If movement is blocked, do nothing, return false
	// Default
	return false;
}


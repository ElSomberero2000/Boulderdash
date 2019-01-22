// Project Includes
#include "Gem.h"
#include "Framework/AssetManager.h"
#include "Level.h"

Gem::Gem()
	: GridObject()
	, fallTime(0.75)
{
	m_sprite.setTexture(AssetManager::GetTexture("graphics/diamond.png"));
	m_blocksMovement = true;
}

bool Gem::AttemptFall(sf::Vector2i _direction)
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
		// Do a dynamic cast to the player to see if we collide
		Player* livingPlayer = dynamic_cast<Player*>(blocker);
		if (livingPlayer != nullptr)
		{
			// DEATH!!!
			m_level->ReloadLevel();
		}
	}

	// If movement is blocked, do nothing, return false
	// Default
	return false;
}

void Gem::Update(sf::Time _frameTime)
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



// Project Includes
#include "Player.h"
#include "Framework/AssetManager.h"
#include "Level.h"
#include "Box.h"
#include "Dirt.h"
#include "Gem.h"

Player::Player()
	: GridObject()
	, m_pendingMove(0, 0)
	, m_moveSound()
	, m_bumpSound()
	, m_gemCount(0)
	

{
	m_sprite.setTexture(AssetManager::GetTexture("graphics/player/playerStandDown.png"));
	m_moveSound.setBuffer(AssetManager::GetSoundBuffer("audio/footstep1.ogg"));
	m_bumpSound.setBuffer(AssetManager::GetSoundBuffer("audio/bump.wav"));
	m_blocksMovement = true;
}

void Player::Input(sf::Event _gameEvent)
{
	// Read the input from the keyboard and convert it
	// to a direction to move in (and then move)

	// Was the event a key press?
	if (_gameEvent.type == sf::Event::KeyPressed)
	{
		// Yes it was a key press!

		// What key was pressed?
		if (_gameEvent.key.code == sf::Keyboard::W)
		{
			// It was W!
			// Move up
			m_pendingMove = sf::Vector2i(0, -1);
			m_sprite.setTexture(AssetManager::GetTexture("graphics/player/playerStandUp.png"));
		}
		else if (_gameEvent.key.code == sf::Keyboard::A)
		{
			// It was A!
			// Move left
			m_pendingMove = sf::Vector2i(-1, 0);
			m_sprite.setTexture(AssetManager::GetTexture("graphics/player/playerStandLeft.png"));
		}
		else if (_gameEvent.key.code == sf::Keyboard::S)
		{
			// It was S!
			// Move down
			m_pendingMove = sf::Vector2i(0, 1);
			m_sprite.setTexture(AssetManager::GetTexture("graphics/player/playerStandDown.png"));
		}
		else if (_gameEvent.key.code == sf::Keyboard::D)
		{
			// It was D!
			// Move right
			m_pendingMove = sf::Vector2i(1, 0);
			m_sprite.setTexture(AssetManager::GetTexture("graphics/player/playerStandRight.png"));
		}
	}
}

// getter to find no. of accumulated gems to display score
int Player::GetGemCount()
{
	return m_gemCount;
}

void Player::Update(sf::Time _frameTime)
{
	// If we have movement waiting to be processed,
	if (m_pendingMove.x != 0 || m_pendingMove.y != 0)
	{
		// move in that direction
		bool moveSuccessful = AttemptMove(m_pendingMove);

		// Play movememnt sound
		if (moveSuccessful == true)
		{
			m_moveSound.play();
		}

		// and clear the pending movement
		m_pendingMove = sf::Vector2i(0, 0);
	}
}


bool Player::AttemptMove(sf::Vector2i _direction)
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
		Dirt* clearedDirt = dynamic_cast<Dirt*>(blocker);
		Gem* collectedGem = dynamic_cast<Gem*>(blocker);

		// If so (the blocker is a box (not nullptr))
		if (pushableBox != nullptr)
		{
			// Attempt to push!
			bool pushSucceeded = pushableBox->AttemptPush(_direction);

			// If push succeeded
			if (pushSucceeded == true)
			{
				// Move to new spot (where blocker was)
				return m_level->MoveObjectTo(this, targetPos);
			}
		}

		// If so (the blocker is dirt (not nullptr))
		if (clearedDirt != nullptr)
		{
			// Attempt to clear!
			m_level->DeleteObjectAt(clearedDirt);

			
			// Move to new spot (where blocker was)
			return m_level->MoveObjectTo(this, targetPos);
			
		}

		// If so (the blocker is a gem (not nullptr))
		if (collectedGem != nullptr)
		{
			// Attempt to clear!
			m_level->DeleteObjectAt(collectedGem);

			// Increase gem count by 1 (for own purpose to help with code)
			m_gemCount++;

			// Only required number is 5 since ghemcount is reset to 0 after each level
			if ( m_gemCount == 5 || m_gemCount == 10 || m_gemCount == 15 || m_gemCount == 20 || m_gemCount == 25)
			{
				m_level->LoadNextLevel();
			}
			else
			{
				// Move to new spot (where blocker was)
				return m_level->MoveObjectTo(this, targetPos);
			}

		
		}
	}

	// If movement is blocked, do nothing, return false
	// Default
	return false;
}
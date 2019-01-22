#pragma once

// Project Includes
#include "GridObject.h"

// Library Includes
#include <SFML/Audio.hpp>

class Box : public GridObject
{

public:

	Box();

	bool AttemptFall(sf::Vector2i _direction);
	bool AttemptSlide(sf::Vector2i _direction);
	void Update(sf::Time _frameTime);
	bool AttemptPush(sf::Vector2i _direction);


private:

	float fallTime;
	float m_timer;
	sf::Vector2i m_pendingMove;
	bool m_stored;
	sf::Sound m_pushSound;
	bool m_falling;
	bool m_sliding;
};
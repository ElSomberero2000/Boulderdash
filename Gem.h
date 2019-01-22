#pragma once

// Project Includes
#include "GridObject.h"

class Gem : public GridObject
{

public:

	Gem();

	bool AttemptFall(sf::Vector2i _direction);
	void Update(sf::Time _frameTime);

private:

	float fallTime;
	float m_timer;

};
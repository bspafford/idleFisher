#pragma once

#include "math.h"

class Aplaceable {
protected:
public: // temp
	// used for inheritence for placable objects
	Aplaceable(Image* img, vector loc);


	// or could i test if the player hits collision?
	// objects would have collision
	// then the object would do stuff if the player coords were inside its length and height
	// just make each building have their owne collision object that they add to the collision list instead
	// then if the player is a close enough, and y > the collision box, then the player is behind
};
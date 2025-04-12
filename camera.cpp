#include "camera.h"
#include "saveData.h"

Ucamera::Ucamera() {
	pos = SaveData::saveData.playerLoc;
	structTest something;
}

void Ucamera::Update(float deltaTime) {
	//pos = SaveData::saveData.playerLoc;
	vector targetPos = SaveData::saveData.playerLoc;

	if (isCameraAttached) {
		float stiffness = 1750;
		float criticalDamping = 2 * sqrt(stiffness * 1);
		float damping = criticalDamping;

		vector force = targetPos - pos;
		force = force * stiffness;
		velocity = (velocity + (force * deltaTime));
		velocity = velocity * exp(-damping * deltaTime);
		//velocity = velocity * pow(damping, deltaTime);

		pos = pos + velocity * deltaTime;
	}

	//pos = targetPos;
}

void Ucamera::detachCamera() {
	isCameraAttached = !isCameraAttached;
}
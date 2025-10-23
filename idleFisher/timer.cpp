#include "timer.h"

#include <iostream>

#include "main.h"
#include "math.h"

timer::timer() {
	instances.push_back(this);
}

timer::~timer() {
	auto it = std::find(instances.begin(), instances.end(), this);
	if (it != instances.end())
		instances.erase(it);
}

// calls update function to all instances of object
void timer::callUpdate(float deltaTime) {
	for (timer* t : instances) {
		if (t) t->Update(deltaTime);
	}
}

void timer::setFps(float fps) {
	maxTime = fps;
}

void timer::Update(float deltaTime) {
	if (!bStart)
		return;

	time += deltaTime;
	time = math::clamp(time, 0, maxTime);
	if (updateCallback_ && time <= maxTime)
		updateCallback_();

	if (time >= maxTime) {
		// call callback function
		stop();
		bFinished = true;
		if (callback_) {
			callback_();
		}
	}
}

float timer::getTime() {
	return time;
}

float timer::getMaxTime() {
	return maxTime;
}

bool timer::finished() {
	return bFinished;
}

void timer::start(float maxTime) {
	bStart = true;
	bFinished = false;

	time = 0;
	this->maxTime = maxTime;
}

void timer::stop() {
	bStart = false;
	time = 0;
}
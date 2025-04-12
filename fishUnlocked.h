#pragma once

#include "widget.h"

class timer;
class animation;
struct FfishData;
struct Fimg;
struct FachievementStruct;
class text;

class UfishUnlocked : public widget {
public:
	UfishUnlocked();
	~UfishUnlocked();
	void draw(SDL_Renderer* renderer);

	void start(FfishData fish);
	void start(FachievementStruct* achievement);
	void setupLocs() override;

private:
	void reverse();
	void finished();

	Fimg* bannerImg;
	animation* anim;
	Fimg* thumbnail;
	text* name;

	timer* finishedTimer;
};
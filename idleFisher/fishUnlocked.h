#pragma once

#include "widget.h"

class timer;
class animation;
struct FfishData;
struct Image;
struct FachievementStruct;
class text;

class UfishUnlocked : public widget {
public:
	UfishUnlocked();
	~UfishUnlocked();
	void draw(Shader* shaderProgram);

	void start(FfishData fish);
	void start(FachievementStruct* achievement);
	void setupLocs() override;

private:
	void reverse();
	void finished();

	Image* bannerImg;
	animation* anim;
	Image* thumbnail;
	text* name;

	timer* finishedTimer;
};
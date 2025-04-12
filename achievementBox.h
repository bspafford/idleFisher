#pragma once

#include "widget.h"

class timer;

struct FachievementStruct;
struct FsaveAchievementStruct;

class UachievementBox : public widget {
public:
	UachievementBox(int id);
	~UachievementBox();

	void draw(SDL_Renderer* renderer);

	vector getSize() override;

	void updateAchievementImage();

private:
	bool mouseOver() override;

	Fimg* icon;

	int id;
	FachievementStruct* achievementStruct;
	FsaveAchievementStruct* saveAchievementStruct;

	bool isMouseOver;
	// was the mouser over last frame
	bool isMouseOverPrev;

	vector startSize;
	static inline float rot;

	static inline timer* rotateTimer;
	static inline float rotateTimerSpeed;
	static void rotateUpdate();
	static void finishRotateTimer();
};
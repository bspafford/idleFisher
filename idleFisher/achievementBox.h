#pragma once

#include "widget.h"

class timer;
struct FachievementStruct;
struct FsaveAchievementStruct;

class UachievementBox : public widget {
public:
	UachievementBox(widget* parent, int id);
	~UachievementBox();

	void draw(Shader* shaderProgram);

	vector getSize() override;

	void updateAchievementImage();

private:
	bool mouseOver() override;

	std::unique_ptr<Image> icon;

	int id;
	FachievementStruct* achievementStruct;
	FsaveAchievementStruct* saveAchievementStruct;

	bool isMouseOver;
	// was the mouser over last frame
	bool isMouseOverPrev;

	vector startSize;
	static inline float rot;

	static inline std::unique_ptr<timer> rotateTimer;
	static inline float rotateTimerSpeed;
	static void rotateUpdate();
	static void finishRotateTimer();
};
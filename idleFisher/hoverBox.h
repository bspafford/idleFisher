#pragma once

#include "widget.h"

class text;
struct FachievementStruct;
struct FsaveAchievementStruct;

class UhoverBox : public widget {
public:
	UhoverBox();
	~UhoverBox();

	void draw(Shader* shaderProgram) override;

	// other is for other infromation, fishing power, price, etc, anchored top right
	void setInfo(std::string name, std::string description, std::string other = " ");

private:
	Image* img;
	text* name;
	text* description;
	text* other;
};
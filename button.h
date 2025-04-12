#pragma once

#include <string>
#include <vector>
#include <stdio.h>
#include <cassert>
#include <functional>

#include "widget.h"
#include "math.h"

class animation;

class Ubutton : public widget{
public:
	// only give NON NULL values to overriding widgets ONLY
	//Ubutton(widget* widget, std::string path, vector loc = { 0, 0 }, std::string hoverPath = "", bool useCharLoc = false, bool useSurface = false);
	Ubutton(widget* widget, std::string spriteSheetPath, int cellWidth, int cellHeight, int numberOfFrames, vector loc, bool useCharLoc, bool useSurface);
	~Ubutton();

	// non static
	template <class T> void addCallback(T* const object, void (T::* const callback) ()) {
		callback_ = std::bind_front(callback, object);
	}

	// static
	void addCallback(void (*callback) ());

	// void setClickAnim(std::vector <std::string> anim, std::vector<std::string> hoverAnim = std::vector<std::string>(0));
	// void removeClickAnim();
	void draw(SDL_Renderer* renderer);

	void onHover(SDL_Renderer* renderer);
	bool isMouseOver();
	void onClick();

	void setLoc(vector loc) override;

	//void setImg(std::string path, std::string hoverPath = "");

	vector getSize();
	vector getLoc();

	void enable(bool enabled);

	void setParent(widget* parent);

	//void setColorMod(int r, int g, int b);
	Fimg* getButtonImg();

	widget* widgetClass;

private:
	std::function<void()> callback_;

	bool bIsHovered = false;

	bool useCharLoc = false;

	bool useSurface;

	bool isEnabled = true;

	SDL_Surface* surface;

	SDL_Color colorMod;

	//
	bool hasHover;
	bool hasDisabled;

public:
	animation* buttonAnim;
};

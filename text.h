#pragma once

#include <string>
#include <vector>
#include <SDL_TTF.h>

#include "math.h"
#include "widget.h"

struct textAlign {
	static inline int left = 0;
	static inline int right = 1;
	static inline int center = 2;
};

struct FtextInfo {
	vector loc = { 0, 0 };
	vector size = { 0, 0 };
};

class text : public widget {
public:
	text(std::string text, std::string font, vector loc, bool useWorldPos = false, bool isometric = false, int alignment = textAlign::left);
	~text();
	void setText(std::string text);
	void draw(SDL_Renderer* renderer);
	// void setAlignment(std::string alignment);
	void makeText(int i, std::string text, vector &offset);
	vector getSize();
	std::string getString();
	void loadTextImg();
	void setTextColor(int r, int g, int b);
	void setupLocs() override;

	// Wraps text if it goes above line length, set to -1 if text doesn't wrap
	void setLineLength(float length);
	float getLineLength();

	void setLocAndSize(vector loc, vector size) override;
	void setLoc(vector loc) override;

	// used when changing the font from pixel to normal or vice versa
	static void changeFontAll();
	void changeFont();
	int calcFontSize();
	Fimg* normFont;

private:
	static inline std::set<text*> instances;

	void makeTextTexture();

	std::string textString;

	// static list of text imgs
	std::vector<Fimg*> letters;

	int alignment;
	std::string font;

	bool isometric = false;
	bool useWorldPos = false;

	float lineLength = -1;

	// start of word
	int start = 0;
	int numLetters = 0;

	// texture that the text draws onto
	SDL_Texture* textTex;

	Fimg* textImg;
	std::vector<FtextInfo> textInfo = std::vector<FtextInfo>(126);//new FtextInfo[126];

	bool alreadySetText = false;
	SDL_Color colorMod = { 255, 255, 255 };
};
#include "text.h"

#include "main.h"
#include "saveData.h"

#include <fstream>
#include <iostream>

// copys the texture from one pointer to another, without being the same reference
SDL_Texture* CopyTexture(SDL_Renderer* renderer, SDL_Texture* original) {
	//return original;
	int width, height;
	SDL_QueryTexture(original, NULL, NULL, &width, &height);
	SDL_Texture* copy = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
	SDL_SetTextureBlendMode(copy, SDL_BLENDMODE_BLEND);
	if (!copy)
		return nullptr;
	SDL_Texture* renderTarget = SDL_GetRenderTarget(renderer);
	SDL_SetRenderTarget(renderer, copy);
	SDL_RenderCopy(renderer, original, NULL, NULL);
	SDL_SetRenderTarget(renderer, renderTarget);
	return copy;
}

text::text(std::string text, std::string font, vector loc, bool useWorldPos, bool isometric, int alignment) {
	instances.insert(this);

	this->textString = text;
	this->alignment = alignment;
	this->font = font;
	this->loc = loc;
	this->isometric = isometric;
	this->useWorldPos = useWorldPos;

	loadTextImg();
	setText(text);
	alreadySetText = true;
}

text::~text() {
	instances.erase(this);

	for (int i = 0; i < letters.size(); i++)
		delete letters[i];
	letters.clear();
	SDL_DestroyTexture(textTex);
	//delete textImg;
	textInfo.clear();
	//textImg = nullptr;
}

void text::changeFontAll() {
	for (text* text : instances) {
		text->changeFont();
	}
}

void text::changeFont() {
	if (!SaveData::settingsData.pixelFont) {
		delete normFont;

		TTF_Font* Sans = TTF_OpenFont("./fonts/ariblk.ttf", calcFontSize());

		SDL_Color White = { 64, 54, 69 };

		SDL_Surface* surfaceMessage = TTF_RenderText_Solid(Sans, textString.c_str(), White);

		int w, h;
		SDL_Texture* Message = SDL_CreateTextureFromSurface(Main::renderer, surfaceMessage);
		SDL_QueryTexture(Message, NULL, NULL, &w, &h);

		normFont = new Fimg(Message, NULL, loc);
		normFont->w = w / stuff::pixelSize;
		normFont->h = h / stuff::pixelSize;

		//

		if (textString == "Probability") {
			std::cout << "hello: " << loc << std::endl;
		}

		SDL_FreeSurface(surfaceMessage);
		// SDL_DestroyTexture(Message);
	} else {
		setText(textString);
	}

}

int text::calcFontSize() {
	if (font == "tall" || font == "straightDark" || font == "straightBoldDark" || font == "straight" || font == "normal" || font == "boldDark" || font == "bold") {
		return 5 * stuff::pixelSize;
	} else {
		return 7 * stuff::pixelSize;
	}
}

void text::loadTextImg() {
	if (textImg)
		delete textImg;
	textImg = new Fimg("./fonts/" + font + "/" + font + ".png");

	// make a list
	// this list would be the size of all char (126)
	// then it would contain the starting location and the size of the char
	// then i could get that infromation by using the char num
	std::ifstream file("./fonts/" + font + "/" + font + ".txt");
	if (!file) {
		std::cout << "no font file\n";
		return;
	}

	std::string line;
	std::string delimiter = " ";
	std::string delimiter1 = ":";

	int lineNum = 0;
	int xOffset = 0;

	// gets the height of the text
	std::string textHeightStr;
	std::getline(file, textHeightStr);
	int textHeight = std::stoi(textHeightStr);

	while (std::getline(file, line)) {
		// get each word in line
		size_t pos = 0;
		std::string token;
		while ((pos = line.find(delimiter)) != std::string::npos || line != "") {
			// includes the last item in list
			if (line.find(delimiter) == std::string::npos)
				pos = line.size();

			char token1;
			size_t pos1 = 0;
			token = line.substr(0, pos);
			if (token.find("32:") != std::string::npos) {
				line.erase(0, pos + delimiter.length());
				if ((pos1 = token.find(":")) != std::string::npos) {
					token.erase(0, pos1 + delimiter1.length());
					textInfo[32].loc = { float(xOffset), float(lineNum) };
				}
				
				token1 = 32;
			} else {
				line.erase(0, pos + delimiter.length());
				while ((pos1 = token.find(":")) != std::string::npos) {
					token1 = *token.substr(0, pos1).c_str();
					token.erase(0, pos1 + delimiter1.length());
					textInfo[token1].loc = { float(xOffset), float(lineNum) };
				}
			}

			xOffset += std::stoi(token);
			textInfo[token1].size = { std::stof(token), float(textHeight) };
		}

		xOffset = 0;
		lineNum += textHeight;
	}
}

void text::setTextColor(int r, int g, int b) {
	colorMod = SDL_Color(r, g, b);
	SDL_SetTextureColorMod(textTex, r, g, b);
}

void text::makeText(int i, std::string text, vector &offset) {
	if (text[i] == ' ') {
		start = i;
		numLetters = 0;
	}

	FtextInfo currInfo = textInfo[text[i]];
	SDL_Rect* source = new SDL_Rect{int(currInfo.loc.x), int(currInfo.loc.y), int(currInfo.size.x), int(currInfo.size.y)};
	// makes a new texture that isn't a reference to the textImg texture
	SDL_Texture* texture = CopyTexture(Main::renderer, textImg->texture);
	letters[i] = new Fimg(texture, source, { 0, 0 });

	Fimg* letter = letters[i];

	if (alignment != textAlign::right)
		letter->loc = offset;

	int temp = 1;
	if (alignment == textAlign::right)
		temp = -1;

	offset.x += letter->w * stuff::pixelSize * temp;

	if (isometric) {
		if (text[i] == '.')
			offset.y -= 1 * stuff::pixelSize * temp;
		else
			offset.y -= 2 * stuff::pixelSize * temp;
	}

	if (alignment == textAlign::right)
		letter->loc = offset;

	// ONLY works for ALIGNED LEFT
	// wraps text if its length is greater than its linelength
	if (lineLength != -1 && offset.x > lineLength) { //  && textString[start] != ' '
		offset.x = 0;
		offset.y += (letter->h + 1) * stuff::pixelSize;
		for (int j = start; j < start + numLetters + 1; j++) {
			letters[j]->loc = offset;
			offset.x += letters[j]->w * stuff::pixelSize;
		}
	}

	std::vector<char> dropList = { 'g', 'p', 'q', 'j', 'y', };
	std::vector<std::string> dontDropFont = { "biggerStraight", "biggerStraightDark" };
	if (std::find(dontDropFont.begin(), dontDropFont.end(), font) == dontDropFont.end() && std::find(dropList.begin(), dropList.end(), text[i]) != dropList.end())
		letters[i]->loc.y += (letters[i]->h / 2.f - 1) * stuff::pixelSize;

	numLetters++;
}

void text::setText(std::string text) {
	if (!SaveData::settingsData.pixelFont) {
		textString = text;
		changeFont();
	} else {
		if (alreadySetText) {
			// when starting its looping through an empty list
			for (int i = 0; i < letters.size(); i++) {
				if (letters[i]) {
					delete letters[i];
					letters[i] = nullptr;
				}
			}
			letters.clear();
		}
		textString = text;
		letters = std::vector<Fimg*>(text.size());// new Fimg* [textSize];
		//for (int i = 0; i < letters.size(); i++) // set list to null
		//	letters[i] = nullptr;

		vector offset = { 0, 0 };
		if (alignment == textAlign::right) {
			for (int i = letters.size() - 1; i >= 0; i--) {
				makeText(i, text, offset);
			}
		} else if (alignment == textAlign::left || alignment == textAlign::center) {
			for (int i = 0; i < letters.size(); i++) {
				makeText(i, text, offset);
			}
		}

		// corrects for chars being out of textures "frame" if isometric, aligned center, or aligned right
		if (alignment != textAlign::left || isometric) {
			if (letters[0] && letters[letters.size() - 1]) {
				vector newLoc = { -letters[0]->loc.x, -letters[letters.size() - 1]->loc.y };
				for (int i = 0; i < letters.size(); i++) {
					letters[i]->loc.x += newLoc.x;
					letters[i]->loc.y += newLoc.y;
				}
			}
		}

		makeTextTexture();
	}
}

void text::makeTextTexture() {
	size = getSize();
	SDL_Renderer* renderer = Main::renderer;
	SDL_Texture* prevRenderTarget = SDL_GetRenderTarget(renderer); // gets previous render target just incase a widget is being cropped
	//SDL_DestroyTexture(textTex);
	textTex = SDL_CreateTexture(Main::renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, size.x, size.y);
	SDL_SetTextureBlendMode(textTex, SDL_BLENDMODE_BLEND);
	SDL_SetRenderTarget(renderer, textTex);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	for (int i = 0; i < letters.size(); i++)
		if (letters[i])
			letters[i]->draw(Main::renderer, false);
	SDL_SetRenderTarget(renderer, prevRenderTarget);
	setTextColor(colorMod.r, colorMod.g, colorMod.b);
}

// 2700 fps
// 4000 fps
void text::draw(SDL_Renderer* renderer) {
	if (!visible)
		return;

	if (!SaveData::settingsData.pixelFont && normFont) {
		normFont->draw(renderer, useWorldPos);
		return;
	}

	vector tempLoc = loc;
	if (useWorldPos)
		tempLoc = math::worldToScreen(tempLoc);

	if (alignment == textAlign::right)
		tempLoc.x -= size.x;

	if (alignment == textAlign::center)
		tempLoc.x -= size.x / 2;

	SDL_Rect rect = SDL_Rect{ int(tempLoc.x), int(tempLoc.y), int(size.x), int(size.y) };
	SDL_RenderCopy(renderer, textTex, NULL, &rect);
}

void text::setLocAndSize(vector loc, vector size) {
	setLoc(loc);
}

void text::setLoc(vector loc) {
	__super::setLoc(loc);
	if (normFont) {
		if (alignment == textAlign::center) {
			normFont->loc = loc - vector{ normFont->getSize().x / 2.f, 0 };
		} else if (alignment == textAlign::right) {
			normFont->loc = loc - vector{ float(normFont->getSize().x), 0 };
		} else
			normFont->loc = loc;
	}
}

vector text::getSize() {
	if (!SaveData::settingsData.pixelFont && normFont) {
		return normFont->getSize();
	} else {

		if (letters.size() == 0)
			return { 0, 0 };

		float minX = INFINITY, minY = INFINITY;
		float maxX = 0, maxY = 0;
		for (int i = 0; i < letters.size(); i++) {
			Fimg* letter = letters[i];
			if (!letter)
				continue;

			if (minX > letter->loc.x)
				minX = letter->loc.x;
			if (maxX < letter->loc.x + letter->w * stuff::pixelSize)
				maxX = letter->loc.x + letter->w * stuff::pixelSize;

			if (minY > letter->loc.y)
				minY = letter->loc.y;
			if (maxY < letter->loc.y + letter->h * stuff::pixelSize)
				maxY = letter->loc.y + letter->h * stuff::pixelSize;
		}
		return { maxX - minX, maxY - minY };
	}
}

void text::setLineLength(float length) {
	lineLength = length;
	setText(textString);
}

std::string text::getString() {
	return textString;
}

float text::getLineLength() {
	return lineLength;
}

void text::setupLocs() {
	setText(textString);
}
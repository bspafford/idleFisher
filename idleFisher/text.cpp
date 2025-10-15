#include "text.h"

#include "main.h"
#include "saveData.h"

#include <fstream>
#include <iostream>

#include "camera.h"

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

	deleteObjects();

	letters.clear();
	textInfo.clear();

	delete textTex;
	textTex = nullptr;

	delete normFont;
	normFont = nullptr;
}

void text::changeFontAll() {
	for (text* text : instances) {
		text->changeFont();
	}
}

void text::changeFont() {
	if (!SaveData::settingsData.pixelFont) {
		/*
		delete normFont;

		TTF_Font* Sans = TTF_OpenFont("./fonts/ariblk.ttf", calcFontSize());

		SDL_Color White = { 64, 54, 69 };

		SDL_Surface* surfaceMessage = TTF_RenderText_Solid(Sans, textString.c_str(), White);

		int w, h;
		SDL_Texture* Message = SDL_CreateTextureFromSurface(Main::shaderProgram, surfaceMessage);
		SDL_QueryTexture(Message, NULL, NULL, &w, &h);

		normFont = Image(Message, NULL, loc);
		normFont->w = w / stuff::pixelSize;
		normFont->h = h / stuff::pixelSize;

		//

		if (textString == "Probability") {
			std::cout << "hello: " << loc << std::endl;
		}

		SDL_FreeSurface(surfaceMessage);
		// SDL_DestroyTexture(Message);
		*/
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
	textImg = std::make_shared<Image>("./fonts/" + font + "/" + font + ".png", vector{ 0, 0 }, useWorldPos);

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
	colorMod = glm::vec4(r / 255.f, g / 255.f, b / 255.f, 1.f);
}

void text::makeText(int i, std::string text, vector &offset) {
	if (text[i] == ' ') {
		start = i;
		numLetters = 0;
	}

	FtextInfo currInfo = textInfo[text[i]];
	std::shared_ptr<Rect> source = std::make_shared<Rect>(currInfo.loc.x, currInfo.loc.y, currInfo.size.x, currInfo.size.y);

	if (textString == "Feed Fish")
		std::cout << *source << std::endl;

	letters[i] = std::make_unique<Image>(textImg, source, vector{0, 0}, false);
	letters[i]->setAnchor(anchor::center, anchor::center);
	Image* letter = letters[i].get();

	if (alignment != textAlign::right)
		letter->setLoc(offset);

	int temp = 1;
	if (alignment == textAlign::right)
		temp = -1;

	offset.x += letter->w * temp * stuff::pixelSize;

	if (isometric) {
		if (text[i] == '.')
			offset.y -= 1 * stuff::pixelSize * temp;
		else
			offset.y -= 2 * stuff::pixelSize * temp;
	}

	if (alignment == textAlign::right)
		letter->setLoc(offset);

	// ONLY works for ALIGNED LEFT
	// wraps text if its length is greater than its linelength
	if (lineLength != -1 && offset.x > lineLength) { //  && textString[start] != ' '
		offset.x = 0;
		offset.y += (letter->h + 1) * stuff::pixelSize;
		for (int j = start; j < start + numLetters + 1; j++) {
			letters[j]->setLoc(offset);
			offset.x += letters[j]->w * stuff::pixelSize;
		}
	}

	std::vector<char> dropList = { 'g', 'p', 'q', 'j', 'y', };
	std::vector<std::string> dontDropFont = { "afScreen", "biggerStraight", "biggerStraightDark" };
	if (std::find(dontDropFont.begin(), dontDropFont.end(), font) == dontDropFont.end() && std::find(dropList.begin(), dropList.end(), text[i]) != dropList.end())
		letters[i]->setLoc((letters[i]->getLoc() + vector{ 0, letters[i]->getSize().y / 2.f - 1 })); // weird problem if i don't subtract 1

	numLetters++;
}

void text::setText(std::string text) {
	if (!SaveData::settingsData.pixelFont) {
		textString = text;
		changeFont();
	} else {
		if (alreadySetText) {
			// when starting its looping through an empty list
			letters.clear();
		}
		textString = text;
		letters = std::vector<std::unique_ptr<Image>>(text.size());

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
				vector _loc = { -letters[0]->getLoc().x, -letters[letters.size() - 1]->getLoc().y};
				for (int i = 0; i < letters.size(); i++) {
					letters[i]->setLoc((letters[i]->getLoc() + _loc));
				}
			}
		}						

		makeTextTexture();
	}
}

void text::deleteObjects() {
	if (currVAO)
		currVAO->Delete();
	if (currEBO)
		currEBO->Delete();

	glDeleteBuffers(1, &VBOId);
	glDeleteTextures(1, &textTexture);
	glDeleteFramebuffers(1, &fbo);

	VBOId = 0;
	textTexture = 0;
	fbo = 0;
}

void text::makeTextTexture() {
	deleteObjects();

	if (letters.size() == 0)
		return;

	fboSize = getSize();

	if (fboSize.x == 0 || fboSize.y == 0)
		return;

	glm::mat4 currProjection = Main::camera->getProjectionMat();

	Main::twoDShader->Activate();
	Main::twoDShader->setMat4("projection", Main::camera->getProjectionMat(fboSize));

	//vector scaledLoc = { 0, 0 };// (-stuff::screenSize / 2.f + vector{ 0, fboSize.y })* vector { 1, -1 }; // need to add fboSize.y cause i flip the axis
	vector scaledLoc = absoluteLoc * vector{ 1, -1 };
	if (useWorldPos)
		scaledLoc = absoluteLoc * stuff::pixelSize;

	float positions[] = {
		// Positions // Texture Coords
		fboSize.x + scaledLoc.x,	scaledLoc.y,				1.f, 1.f,	// Bottom-right
		fboSize.x + scaledLoc.x,	fboSize.y + scaledLoc.y,	1.f, 0.f,	// Top-right // idk why the y texCoords have to be flipped
		scaledLoc.x,				fboSize.y + scaledLoc.y,	0.f, 0.f,	// Top-left
		scaledLoc.x,				scaledLoc.y,				0.f, 1.f	// Bottom-left
	};

	std::vector<GLuint> indices = {
		0, 1, 3, // First triangle
		3, 1, 2  // Second triangle
	};

	currVAO = std::make_unique<VAO>();
	currVAO->Bind();

	glGenBuffers(1, &VBOId);
	glBindBuffer(GL_ARRAY_BUFFER, VBOId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

	currEBO = std::make_unique<EBO>(indices);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	//currVBO->Unbind();
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Create Texture
	glGenTextures(1, &textTexture);
	glBindTexture(GL_TEXTURE_2D, textTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, fboSize.x, fboSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Create FBO
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glViewport(0, 0, fboSize.x, fboSize.y);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textTexture, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cerr << "FBO is incomplete!" << std::endl;
	}

	// Render to the FBO

	// draw stuff here
	//glClearColor(0, 0, 0, 1);
	//glClear(GL_COLOR_BUFFER_BIT);
	for (int i = 0; i < letters.size(); i++)
		if (letters[i]) {
			letters[i]->setLoc((letters[i]->getLoc() - fboSize / 2.f + letters[i]->getSize() / 2.f)); // positions top left of frame buffer
			letters[i]->draw(Main::twoDShader);
		}

	// Unbind FBO
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, stuff::screenSize.x, stuff::screenSize.y);

	setLoc(loc);

	// restores projection
	Main::twoDShader->setMat4("projection", currProjection);
}

void text::draw(Shader* shaderProgram) {
	/*
	if (!visible)
		return;

	if (!SaveData::settingsData.pixelFont && normFont) {
		normFont->draw(shaderProgram);
		return;
	}

	vector tempLoc = loc;
	if (useWorldPos) {
		tempLoc = math::worldToScreen(tempLoc, "topleft");
	}

	if (alignment == textAlign::right)
		tempLoc.x -= size.x;

	if (alignment == textAlign::center)
		tempLoc.x -= size.x / 2;

	setLoc(tempLoc);
	*/

	drawTexture(shaderProgram, textTexture);

	/*
	for (int i = 0; i < letters.size(); i++)
		if (letters[i])
			letters[i]->draw(Main::twoDShader);
	setTextColor(colorMod.r, colorMod.g, colorMod.b);
	//*/

	//letters[0]->draw(Main::twoDShader);

	// now render the fbo elsewhere
}

void text::setLocAndSize(vector loc, vector size) {
	setLoc(loc);
}

void text::setLoc(vector loc) {
	__super::setLoc(loc);

	// this is the non pixel font
	if (normFont) {
		if (alignment == textAlign::center) {
			this->loc = loc - vector{ (normFont->getSize().x / 2.f), 0 };
			normFont->setLoc(loc - vector{ (normFont->getSize().x / 2.f), 0 });
		} else if (alignment == textAlign::right) {
			this->loc = loc - vector{ (normFont->getSize().x), 0 };
			normFont->setLoc(loc - vector{ (normFont->getSize().x), 0 });
		} else {
			this->loc = loc;
			normFont->setLoc(loc);
		}
	}

	if (useWorldPos) {
		vector size = getSize();
		vector halfScreen = (stuff::screenSize / 2.f);

		if (alignment == textAlign::left) {
			absoluteLoc = loc;
			if (isometric)
				absoluteLoc += vector{ 0, size.y / stuff::pixelSize };
		} else if (alignment == textAlign::right) {
			absoluteLoc = loc - vector{ size.x / stuff::pixelSize, 0 };
			if (isometric)
				absoluteLoc -= vector{ 0, getSize().y / stuff::pixelSize };
		} else if (alignment == textAlign::center) {
			absoluteLoc = loc - vector{ getSize().x / 2.f, 0.f };
		}

		updatePositionsList();
	} else {
		vector size = getSize();
		vector halfScreen = (stuff::screenSize / 2.f);

		// convert { 0, 0 } from center to top left of screen and makes the top left the { 0, 0 } of the text
		loc += (-halfScreen + vector{0, size.y});

		if (alignment == textAlign::left) {
			absoluteLoc = loc;
		} else if (alignment == textAlign::right) {
			absoluteLoc = loc - vector{ getSize().x, 0.f };
		} else if (alignment == textAlign::center) {
			absoluteLoc = loc - vector{ (getSize().x / 2.f), 0.f };
		}

		updatePositionsList();
	}
}

void text::updatePositionsList() {
	if (!currVAO)
		return;

	currVAO->Bind();
	glBindBuffer(GL_ARRAY_BUFFER, VBOId);

	float x1 = 0;
	float x2 = 1;
	float y1 = 0;
	float y2 = 1;

	vector scale = getSize();
	vector scaledLoc = absoluteLoc * vector{ 1, -1 };
	if (useWorldPos)
		scaledLoc = absoluteLoc * stuff::pixelSize;
	float positions[] = {
		// Positions // Texture Coords
		fboSize.x + scaledLoc.x,	scaledLoc.y,				1.f, 0.f,	// Bottom-right
		fboSize.x + scaledLoc.x,	fboSize.y + scaledLoc.y,	1.f, 1.f,	// Top-right // idk why the y texCoords have to be flipped
		scaledLoc.x,				fboSize.y + scaledLoc.y,	0.f, 1.f,	// Top-left
		scaledLoc.x,				scaledLoc.y,				0.f, 0.f	// Bottom-left
	};

	// updates the tex coords
	void* ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	if (ptr) {
		memcpy(ptr, positions, sizeof(positions)); // Copy updated vertex data
		glUnmapBuffer(GL_ARRAY_BUFFER);
	}
}

void text::setAnchor(std::string xAnchor, std::string yAnchor) {
	if (useWorldPos) {
		std::cout << "This is a world object, it doesn't work";
		return;
	}

	this->xAnchor = xAnchor;
	this->yAnchor = yAnchor;
	setLoc(loc);
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
			Image* letter = letters[i].get();
			if (!letter)
				continue;

			if (minX > letter->getLoc().x)
				minX = letter->getLoc().x;
			if (maxX < letter->getLoc().x + letter->w * stuff::pixelSize)
				maxX = letter->getLoc().x + letter->w * stuff::pixelSize;

			if (minY > letter->getLoc().y)
				minY = letter->getLoc().y;
			if (maxY < letter->getLoc().y + letter->h * stuff::pixelSize)
				maxY = letter->getLoc().y + letter->h * stuff::pixelSize;
		}
		return { maxX - minX, maxY - minY };
	}

	return { 0, 0 };
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

void text::drawTexture(Shader* shaderProgram, GLuint textureID) {
	/*
	if (textString == "10.6k")
		for (int i = 0; i < letters.size(); i++)
			letters[i]->draw(shaderProgram);
	return;
	*/
	if (!currVAO)
		return;

	shaderProgram->Activate();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	currVAO->Bind();

	glUniform1i(glGetUniformLocation(shaderProgram->ID, "useWorldPos"), useWorldPos);
	shaderProgram->setVec4("color", colorMod);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	currVAO->Unbind();
}
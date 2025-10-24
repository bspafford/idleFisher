#pragma once

#include <string>
#include <vector>
#include <mutex>

#include "math.h"
#include "widget.h"
#include "Image.h"

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
	text(widget* parent, std::string text, std::string font, vector loc, bool useWorldPos = false, bool isometric = false, int alignment = textAlign::left);
	~text();
	void setText(std::string text);
	void draw(Shader* shaderProgram);
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
	// anchor::...
	void setAnchor(std::string xAnchor, std::string yAnchor);

	void updatePositionsList();

	// deletes VAO, VBO, EBO, TEXTURE, FBO
	void deleteObjects();

	// used when changing the font from pixel to normal or vice versa
	static void changeFontAll();
	void changeFont();
	int calcFontSize();
	Image* normFont;

	void makeTextTexture();
private:
	static inline std::vector<text*> instances;

	std::string textString;

	// static list of text imgs
	std::vector<std::unique_ptr<Image>> letters;

	int alignment;
	std::string font;

	bool isometric = false;
	bool useWorldPos = false;

	float lineLength = -1;

	// start of word
	int start = 0;
	int numLetters = 0;

	// texture that the text draws onto
	unsigned char* textTex;

	std::shared_ptr<Image> textImg;
	std::vector<FtextInfo> textInfo = std::vector<FtextInfo>(126);

	glm::vec4 colorMod = glm::vec4(1.f);

	// text fbo
	GLuint fbo = 0;
	GLuint textTexture = 0;
	vector fboSize;

	void drawTexture(Shader* shaderProgram, GLuint textureID);
	GLuint VBOId = 0;
	std::unique_ptr<VAO> currVAO;
	std::unique_ptr<EBO> currEBO;

	vector absoluteLoc;
	std::string xAnchor = anchor::left;
	std::string yAnchor = anchor::top;

	int textHeight;
};
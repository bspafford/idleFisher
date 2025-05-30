#pragma once

#include "widget.h"

class Ubutton;
class text;
class UsailorWidget;
class animation;

class Umap : public widget {
public:
	Umap(UsailorWidget* parent);
	void draw(SDL_Renderer* renderer) override;

	void setLoc(vector loc) override;
	vector getLoc() override;

private:
	void moveMap();
	void setLocs(vector loc);

	Fimg* mapImg;

	UsailorWidget* parent;

	bool mouseDown = false;
	vector mouseStartPos;
	vector imgStartPos;

	animation* hereBoat;
	text* hereText;

	std::vector<Ubutton*> worldButtons;
	std::vector<vector> worldButtonLoc = { {93, 82}, {90, 224}, {227, 241}, {331, 177}, {467, 328}, {397, 406}, {403, 473}, {259, 424}, {87, 458}, {117, 392} };
	std::vector<text*> worldNames;
	std::vector<Fimg*> worldLines;
	std::vector<vector> worldLineLoc = { {398, 297}, {425, 415}, {557, 381}, {619, 387}, {722, 548}, {721, 614}, {588, 631}, {419, 619}, {390, 608} };

	void openWorld1() { std::cout << "is it working?" << std::endl; openLevel("world1"); }
	void openWorld2() { openLevel("world2"); }
	void openWorld3() { openLevel("world3"); }
	void openWorld4() { openLevel("world4"); }
	void openWorld5() { openLevel("world5"); }
	void openWorld6() { openLevel("world6"); }
	void openWorld7() { openLevel("world7"); }
	void openWorld8() { openLevel("world8"); }
	void openWorld9() { openLevel("world9"); }
	void openWorld10() { openLevel("world10"); }
	void openLevel(std::string levelName);
};
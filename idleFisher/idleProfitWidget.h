#pragma once

#include "widget.h"
#include "saveData.h"

class Ubutton;
class text;
class UheldFishWidget;
class UscrollBox;

class UidleProfitWidget : public widget {
public:
	UidleProfitWidget(widget* parent);
	~UidleProfitWidget();
	void draw(Shader* shaderProgram) override;
	void setup(std::vector<FsaveFishData> fishList);
private:
	void addedToViewport() override;
	void setupLocs() override;

	// components
	std::unique_ptr<URectangle> background;
	std::unique_ptr<Ubutton> collectButton;
	std::unique_ptr<text> title;
	std::unique_ptr<text> collectText;

	std::unique_ptr<UscrollBox> scrollBox;
	std::unique_ptr<UheldFishWidget> heldFishWidget;
};
#pragma once

#include "widget.h"

class UdecoratorWidget;
class Ubutton;
class text;
struct FvaultUnlocksStruct;
struct FsaveVaultUnlocksStruct;

class UdecorationBox : public widget {
public:
	UdecorationBox(widget* parent, UdecoratorWidget* parentRef, widget* buttonParent, FvaultUnlocksStruct* vaultUnlock, FsaveVaultUnlocksStruct* saveVaultUnlock);
	~UdecorationBox();
	void draw(Shader* shaderProgram);
	void setLoc(vector loc) override;
	vector getSize() override;
	void buyDecoration();
	void updateWidget();

private:
	UdecoratorWidget* parentRef;

	FvaultUnlocksStruct* vaultUnlock;
	FsaveVaultUnlocksStruct* saveVaultUnlock;

	std::unique_ptr<Image> thumbnail;
	std::unique_ptr<Ubutton> buyButton;
	std::unique_ptr<text> buttonText;
};
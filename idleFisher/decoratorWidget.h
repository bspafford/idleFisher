#pragma once

#include "widget.h"

class UscrollBox;
class Adecorator;
class Ubutton;
class text;
class verticalBox;
class UwrapBox;
class npc;
struct FvaultUnlocksStruct;
struct FsaveVaultUnlocksStruct;
class Rectangle;

class UdecoratorWidget : public widget {
public:
	UdecoratorWidget(Adecorator* parent);
	~UdecoratorWidget();

	void draw(Shader* shaderProgram) override;
	void setupLocs() override;
	void addedToViewport() override;
	void placeItem(FvaultUnlocksStruct* vaultUnlock, FsaveVaultUnlocksStruct* saveVaultUnlock);
	void removeItem(FvaultUnlocksStruct* vaultUnlock, FsaveVaultUnlocksStruct* saveVaultUnlock);
	void moveMode();

private:
	void finishPlaceMode();

	npc* parent;

	// upgrades
	UscrollBox* scrollBox;
	UwrapBox* wrapBox;
	Rectangle* upgradeBackground;

	// stats
	verticalBox* nameHolder;
	text* name;
	text* description;
	Rectangle* infoBackground;

	// npc img
	Image* npcImg;
	Rectangle* npcBackground;

	Ubutton* closeButton;

	// placing items
	Ubutton* finishPlacingButton;
	Ubutton* moveButton;
};
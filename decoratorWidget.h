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

class UdecoratorWidget : public widget {
public:
	UdecoratorWidget(Adecorator* parent);
	~UdecoratorWidget();

	void draw(SDL_Renderer* renderer) override;
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
	SDL_Rect* upgradeBackground;

	// stats
	verticalBox* nameHolder;
	text* name;
	text* description;
	SDL_Rect* infoBackground;

	// npc img
	Fimg* npcImg;
	SDL_Rect* npcBackground;

	Ubutton* closeButton;

	// placing items
	Ubutton* finishPlacingButton;
	Ubutton* moveButton;
};
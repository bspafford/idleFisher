#pragma once

#include "widget.h"

class UdecoratorWidget;
class Ubutton;
class text;
struct FvaultUnlocksStruct;
struct FsaveVaultUnlocksStruct;

class UdecorationBox : public widget {
public:
	UdecorationBox(UdecoratorWidget* parentRef, widget* buttonParent, FvaultUnlocksStruct* vaultUnlock, FsaveVaultUnlocksStruct* saveVaultUnlock);
	~UdecorationBox();
	void draw(SDL_Renderer* renderer);
	void setLoc(vector loc) override;
	vector getSize() override;
	void buyDecoration();
	void updateWidget();

private:
	UdecoratorWidget* parentRef;

	FvaultUnlocksStruct* vaultUnlock;
	FsaveVaultUnlocksStruct* saveVaultUnlock;

	Fimg* thumbnail;
	Ubutton* buyButton;
	text* buttonText;
};
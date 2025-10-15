#include "comboWidget.h"

#include "main.h"
#include "math.h"
#include "stuff.h"
#include "shortNumbers.h"
#include "character.h"
#include "text.h"

UcomboWidget::UcomboWidget() {
	comboText = std::make_unique<text>(" ", "biggerStraight", vector{ 0, 0 }, false, false, textAlign::center);

	spawnComboNumber(1);
}

UcomboWidget::~UcomboWidget() {

}

void UcomboWidget::draw(Shader* shaderProgram) {
	if (!visible)
		return;

	//SDL_Rect rect = SDL_Rect{ (stuff::screenSize.x - text->w) / 2, int(4 * stuff::pixelSize), text->w, text->h };
	//SDL_Rect rect = SDL_Rect{ stuff::screenSize.x / 2, int(4 * stuff::pixelSize), text->w, text->h };
	comboText->draw(shaderProgram);
	//SDL_RenderCopy(shaderProgram, text->texture, NULL, &rect);
}

void UcomboWidget::update(float deltaTime) {
	/*
	if (fading) {
		fadeTimer += deltaTime;
		horizontalBox->SetRenderOpacity(1 - fadeTimer / fadeTimerMax);
		if (fadeTimer >= fadeTimerMax) {
			fadeTimer = 0;
			fading = false;
			comboText->SetRenderTranslation(FVector2D(0, 0));
			xText->SetRenderTranslation(FVector2D(0, 0));
			horizontalBox->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	*/
}
void UcomboWidget::spawnComboNumber(int comboNum) {
	/*
	TTF_Font* Sans = TTF_OpenFont("./fonts/pixelFont.ttf", 5 * stuff::pixelSize);
	SDL_Color White = { 255, 255, 255 };
	SDL_Surface* surfaceText = TTF_RenderText_Solid(Sans, ("x" + shortNumbers::convert2Short(comboNum)).c_str(), White);
	text->texture = SDL_CreateTextureFromSurface(Main::shaderProgram, surfaceText);
	SDL_QueryTexture(text->texture, NULL, NULL, &text->w, &text->h);

	TTF_CloseFont(Sans);
	SDL_FreeSurface(surfaceText);
	*/
	// comboText->SetRenderTranslation(FVector2D(0, 0));
	// comboText->SetRenderTransformAngle(0);
	comboText->setText("x" + shortNumbers::convert2Short(comboNum));
	setupLocs();

	if (comboNum == 1)
		hideComboText();
	else if (Main::character->isFishing)
		showComboText();

	/*
	if (comboNum > prevComboNum) { // green
		comboText->SetText(FText::FromString(FString::Printf(TEXT("%d"), comboNum)));
		tempIncrease();
	} else if (comboNum == prevComboNum) { // yellow
		comboText->SetText(FText::FromString(FString::Printf(TEXT("%d"), comboNum)));
		tempSame();
	} else { // blue
		fading = true;
		tempBreak();
	}
	*/

	prevComboNum = comboNum;
}

void UcomboWidget::setupLocs() {
	comboText->setLoc({ stuff::screenSize.x / 2.f, 4 * stuff::pixelSize });
}

void UcomboWidget::showComboText() {
	if (Main::character->isFishing)
		visible = true;
}

void UcomboWidget::hideComboText() {
	// slowly fade
	//fading = true;
	visible = false;
}
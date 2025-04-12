#include "hoverBox.h"

#include "main.h"
#include "saveData.h"

#include "text.h"

UhoverBox::UhoverBox() {
	img = new Fimg("./images/widget/hoverImg.png", Main::mousePos); //new SDL_Rect{ int(Main::mousePos.x), int(Main::mousePos.y), int(size.x), int(size.y)};
	name = new text(" ", "straight", {0, 0});
	description = new text(" ", "straight", { 0, 0 });
	description->setLineLength(250);
	other = new text(" ", "straight", { 0, 0 }, false, false, textAlign::right);
	size = img->getSize();
}

UhoverBox::~UhoverBox() {
	delete img;
	delete name;
	delete description;
	delete other;
	img = nullptr;
	name = nullptr;
	description = nullptr;
	other = nullptr;
}

void UhoverBox::draw(SDL_Renderer* renderer) {
	if (!visible)
		return;
	// if right is off screen, add offset
	// same for bottom

	//std::cout << (Main::mousePos.x + (size.x + 7) * stuff::pixelSize) << ", " << Main::screenWidth << std::endl;
	if (Main::mousePos.x + size.x + 7 * stuff::pixelSize >= Main::screenWidth) // out of range on right
		img->loc.x = int(Main::mousePos.x - size.x - 7 * stuff::pixelSize);
	else
		img->loc.x = int(Main::mousePos.x + 7 * stuff::pixelSize);

	if (Main::mousePos.y + size.y + 7 * stuff::pixelSize >= Main::screenHeight) // out of range on bottom
		img->loc.y = int(Main::mousePos.y - size.y - 7 * stuff::pixelSize);
	else
		img->loc.y = int(Main::mousePos.y + 7 * stuff::pixelSize);

	name->setLoc(img->loc + 5 * stuff::pixelSize);
	description->setLoc({ img->loc + vector{ 5, 11 } * stuff::pixelSize });
	other->setLoc(img->loc + vector{ img->getSize().x, 0 } + vector{ -5, 5 } *stuff::pixelSize);

	//SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
	//SDL_RenderFillRect(renderer, rect);
	img->draw(renderer, false);
	name->draw(renderer);
	description->draw(renderer);
	other->draw(renderer);
}

void UhoverBox::setInfo(std::string newName, std::string newDescription, std::string newOther) {
	if (name->getString() == newName && description->getString() == newDescription && other->getString() == newOther) // if still the same data
		return;

	name->setText(newName);
	description->setText(newDescription);
	other->setText(newOther);
}
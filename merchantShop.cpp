#include "merchantShop.h"

AmerchantShop::AmerchantShop(vector loc) : Abuilding(loc) {
	buildingImg = new Fimg("./images/merchantShop.png", loc);

	setupCollision({ {498,-286}, {577,-323}, {605,-305}, {534,-267} });
}

AmerchantShop::~AmerchantShop() {

}

void AmerchantShop::draw(SDL_Renderer* renderer) {
	__super::draw(renderer);
}
#include "petShop.h"

ApetShop::ApetShop(vector loc) : Abuilding(loc) {
	buildingImg = new Fimg("./images/petShop.png", loc);
}

ApetShop::~ApetShop() {

}

void ApetShop::draw(SDL_Renderer* renderer) {
	__super::draw(renderer);
}
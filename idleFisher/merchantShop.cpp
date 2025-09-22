#include "merchantShop.h"

AmerchantShop::AmerchantShop(vector loc) : Abuilding(loc) {
	buildingImg = new Image("./images/merchantShop.png", loc, true);

	setupCollision({ loc + vector{0, 19}, loc + vector{36, 0}, loc + vector{106, 35}, loc + vector{70, 54} });
	//setupCollision({ loc + vector{0, 0}, loc + vector{100, 0}, loc + vector{100, 100}, loc + vector{0, 100} });
}

AmerchantShop::~AmerchantShop() {

}

void AmerchantShop::draw(Shader* shaderProgram) {
	__super::draw(shaderProgram);
}
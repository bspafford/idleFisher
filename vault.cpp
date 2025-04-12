#include "vault.h"
#include "main.h"
#include "worlds.h"
#include "button.h"

Avault::Avault(vector loc) {
	this->loc = loc;
	//vaultButton = new Ubutton(NULL, "./images/vault.png", loc, "", true, true);
	//vaultButton->addCallback(this, &Avault::enterVault);
}

void Avault::draw(SDL_Renderer* renderer) {
	if (vaultButton)
		vaultButton->draw(renderer);
}

void Avault::click() {
	Main::addLeftClick(this, &Avault::enterVault);
}

void Avault::enterVault() {
	std::cout << "Main: " << Main::currWorld << ", " << Main::prevWorld << std::endl;
	// temp
	if (Main::currWorld != "vault")
		Main::openLevel("vault");
	else
		Main::openLevel(Main::prevWorld, worldLoc::leaveVault);
}

void Avault::setLoc(vector loc) {
	this->loc = loc;
	if (vaultButton)
		vaultButton->setLoc(loc);
}
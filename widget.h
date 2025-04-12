#pragma once

#include <SDL.h>
#include <set>

#include "math.h"

class widget {
protected:
	widget();
	virtual ~widget();

	bool visible = true;

	static inline std::set<widget*> instances;

	// called when resizing screen
	virtual void setupLocs();

	// calls when widget is added to viewport
	virtual void addedToViewport();

public:
	virtual void draw(SDL_Renderer* renderer);

	virtual void addToViewport(bool override = false);

	bool isVisible();
	void setVisibility(bool visible);

	void removeFromViewport();

	virtual bool mouseOver();

	static void resizeScreen();

	// loc and size
	virtual vector getLoc();
	virtual vector getSize();
	virtual void setLoc(vector loc);
	virtual void setOgLoc(vector loc);
	virtual void setSize(vector size);
	virtual void setLocAndSize(vector loc, vector size);

	vector loc = { 0, 0 };
	// the original location of a widget, used for scrollboxes and wrapboxes
	vector ogLoc = { 0, 0 };
	vector size = { 0, 0 };

	widget* parent = NULL;
};
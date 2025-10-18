#pragma once

#include <set>

#include "math.h"
#include "shaderClass.h"
#include "Image.h"
#include "stuff.h"
#include "Rectangle.h"

class widget {
protected:
	widget(widget* parent);
	virtual ~widget();

	bool visible = true;

	static inline std::set<widget*> instances;

	// called when resizing screen
	virtual void setupLocs();

	// calls when widget is added to viewport
	virtual void addedToViewport();

public:
	virtual void draw(Shader* shaderProgram);

	virtual void addToViewport(bool override = false);

	bool isVisible();
	void setVisibility(bool visible);

	virtual void removeFromViewport();

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

	widget* getParent();
	void setParent(widget* parent);
private:
	widget* parent = NULL;
};
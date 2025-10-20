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

	// returns the override widget if there is one
	static widget* getCurrWidget();

	widget* getParent();
	void setParent(widget* newParent);
	// returns root parent variable
	widget* getRootParent();
	void updateAllChildrensRootParent(widget* curr, widget* newRootParent);
private:
	widget* parent = nullptr;
	widget* rootParent = nullptr;
	// calculates the root parent from this objects position
	widget* recalcRootParent();
	void setRootParent(widget* rootParent);
	std::vector<widget*> children;

	// is the overriding widget that is open on screen
	// something like the journal, NPC menus, settings, etc
	static inline widget* currWidget;
};
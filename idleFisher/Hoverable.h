#pragma once

class IHoverable {
public:
	virtual ~IHoverable() = default;

	// set what the mouse hover icon should be
	void setMouseHoverIcon(std::string mouseName) {
		mouse = mouseName;
	}
	std::string getMouseHoverIcon() {
		return mouse;
	}

private:
	// which mouse should be active on hover
	std::string mouse = "cursor1";
};
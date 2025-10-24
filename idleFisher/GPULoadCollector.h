#pragma once

#include <iostream>
#include <mutex>

#include "Image.h"
#include "animation.h"
#include "text.h"
#include "Rectangle.h"

class GPULoadCollector {
public:
	static void open() {
		std::lock_guard<std::mutex> lock(mutex);
		imagesToUpload.clear();
		animationsToUpload.clear();
		textToUpload.clear();
		rectToUpload.clear();
		active = true;
	}

	static void setMainThread(std::thread::id mainThread) {
		std::lock_guard<std::mutex> lock(mutex);
		mainThreadId = mainThread;
	}

	static bool isOnMainThread() {
		return std::this_thread::get_id() == mainThreadId;
	}

	static void add(Image* img) {
		std::lock_guard<std::mutex> lock(mutex);
		if (active && !isOnMainThread())
			imagesToUpload.push_back(img);
		else
			img->loadGPU();
	}

	static void add(animation* anim) {
		std::lock_guard<std::mutex> lock(mutex);
		if (active && !isOnMainThread())
			animationsToUpload.push_back(anim);
	}

	static void add(text* text) {
		std::lock_guard<std::mutex> lock(mutex);
		if (active && !isOnMainThread())
			textToUpload.push_back(text);
	}

	static void add(URectangle* rect) {
		std::lock_guard<std::mutex> lock(mutex);
		if (active && !isOnMainThread())
			rectToUpload.push_back(rect);
		else
			rect->loadGPU();
	}

	static void close(std::vector<Image*>& images, std::vector<animation*>& animations, std::vector<text*>& text, std::vector<URectangle*>& rect) {
		std::lock_guard<std::mutex> lock(mutex);
		active = false;
		images = imagesToUpload;
		animations = animationsToUpload;
		text = textToUpload;
		rect = rectToUpload;

		imagesToUpload.clear();
		animationsToUpload.clear();
		textToUpload.clear();
		rectToUpload.clear();
	}

private:
	static inline std::vector<Image*> imagesToUpload;
	static inline std::vector<animation*> animationsToUpload;
	static inline std::vector<text*> textToUpload;
	static inline std::vector<URectangle*> rectToUpload;
	static inline bool active = false;
	static inline std::mutex mutex;
	static inline std::thread::id mainThreadId;
};
/*==============================================================================

	Visual: a simple, osc-controlled graphics & scripting engine
  
	Copyright (c) 2013 Dan Wilcox <danomatika@gmail.com>

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program. If not, see <http://www.gnu.org/licenses/>.
	
	See https://github.com/danomatika/Visual for documentation

==============================================================================*/
#include "Video.h"

//--------------------------------------------------------------
Video::Video(string name) : DrawableObject(name),
	bPlay(false), volume(0), speed(1), loopType(OF_LOOP_NORMAL),
	pos(0, 0), width(0), height(0), bDrawFromCenter(false) {
	clear();
}

//--------------------------------------------------------------
Video::Video(string name, string filename) : DrawableObject(name),
	bPlay(false), volume(0), speed(1), loopType(OF_LOOP_NORMAL),
	pos(0, 0), width(0), height(0), bDrawFromCenter(false), filename(filename) {
	clear();
}

//--------------------------------------------------------------
bool Video::loadFile(string filename) {
	if(filename == "") {
		filename = this->filename;
	}
	
	bool loaded = false;
	string baseName = ofFilePath::getBaseName(filename);
	if(!Config::instance().resourceManager.videoExists(baseName)) {
		if(!Config::instance().resourceManager.addVideo(baseName, filename)) {
			ofLogWarning() << "Video: \"" << name << "\" couldn't load \""
				<< filename << "\"";
			return false;
		}
		loaded = true;
		this->filename = filename;
	}
	video = Config::instance().resourceManager.getVideo(baseName);

	if(loaded) {
		ofLogVerbose(PACKAGE) << "Video: loaded \"" << baseName << "\" "
				<< video->getWidth() << "x" << video->getHeight();
	}
	
	// get dimen from video if not set
	if(width == 0)	width = video->getWidth();
	if(height == 0) height = video->getHeight();
	
	// apply settings
	if(bPlay) {
		video->play();
	}
	video->setVolume(volume);
	video->setSpeed(speed);
	video->setLoopState(loopType);

	return true;
}

//--------------------------------------------------------------
void Video::setup() {
	if(!video->isLoaded()) {
		string baseName = ofFilePath::getBaseName(filename);
		if(Config::instance().resourceManager.videoExists(baseName)) {
			video = Config::instance().resourceManager.getVideo(baseName);
		}
		else {
			loadFile();
		}
	}
}

//--------------------------------------------------------------
void Video::update() {
	if(video->isLoaded()) {
		video->update();
	}
}

//--------------------------------------------------------------
void Video::draw() {
	draw(pos.x, pos.y, width, height);
}

//--------------------------------------------------------------
void Video::draw(int x, int y) {
	draw(x, y, width, height);
}

//--------------------------------------------------------------
void Video::draw(int x, int y, unsigned int w, unsigned int h) {
	if(!video->isLoaded() || !bVisible) {
		return;
	}

	ofSetColor(color);
	if(bDrawFromCenter) {
		video->draw(x-w/2, y-h/2, w, h);
	}
	else {
		video->draw(x, y, w, h);
	}
}

//--------------------------------------------------------------
void Video::clear() {
	if(video.get() != NULL) {
		video->stop();
		video->closeMovie();
	}
	video = ofPtr<ofVideoPlayer>(new ofVideoPlayer); // empty player
	color.set(255);
}

//--------------------------------------------------------------
void Video::nextFrame() {
	video->nextFrame();
}

//--------------------------------------------------------------
void Video::prevFrame() {
	video->previousFrame();
}

//--------------------------------------------------------------
void Video::setPlay(bool b) {
	if(bPlay == b) {
		return;
	}
	bPlay = b;
	if(bPlay) {
		video->play();
	}
	else {
		video->stop();
	}
}

//--------------------------------------------------------------
void Video::setVolume(float v) {
	volume = ofClamp(v, 0, 1);
	video->setVolume(volume);
}

//--------------------------------------------------------------
float Video::getSpeed() {
	return video->getSpeed();
}

//--------------------------------------------------------------
void Video::setSpeed(float s) {
	speed = s;
	video->setSpeed(speed);
}

//--------------------------------------------------------------
ofLoopType Video::getLoop() {
	video->getLoopState();
}

//--------------------------------------------------------------
void Video::setLoop(ofLoopType t) {
	loopType = t;
	video->setLoopState(loopType);
}

//--------------------------------------------------------------
void Video::setSize(unsigned int w, unsigned int h) {
	width = w;
	height = h;
}

//--------------------------------------------------------------
bool Video::processOscMessage(const ofxOscMessage& message) {

	// call the base class
	if(DrawableObject::processOscMessage(message)) {
		return true;
	}

	else if(message.getAddress() == oscRootAddress + "/play") {
		bool b = bPlay;
		if(tryBool(message, b, 0)) {
			setPlay(b);
		}
		return true;
	}
	else if(message.getAddress() == oscRootAddress + "/volume") {
		float v = 0;
		if(tryNumber(message, v, 0)) {
			setVolume(v);
		}
		return true;
	}
	else if(message.getAddress() == oscRootAddress + "/speed") {
		float s = 0;
		if(tryNumber(message, s, 0)) {
			setSpeed(s);
		}
		return true;
	}
	else if(message.getAddress() == oscRootAddress + "/loop") {
		unsigned int l = (unsigned int) OF_LOOP_NONE;
		if(tryNumber(message, l, 0)) {
			setLoop((ofLoopType) l);
		}
		return true;
	}

	if(message.getAddress() == oscRootAddress + "/position") {
		tryNumber(message, pos.x, 0);
		tryNumber(message, pos.y, 1);
		return true;
	}
	else if(message.getAddress() == oscRootAddress + "/position/x") {
		tryNumber(message, pos.x, 0);
		return true;
	}
	else if(message.getAddress() == oscRootAddress + "/position/y") {
		tryNumber(message, pos.y, 0);
		return true;
	}
	
	
	else if(message.getAddress() == oscRootAddress + "/size") {
		tryNumber(message, width, 0);
		tryNumber(message, height, 1);
		return true;
	}
	else if(message.getAddress() == oscRootAddress + "/size/width") {
		tryNumber(message, width, 0);
		return true;
	}
	else if(message.getAddress() == oscRootAddress + "/size/height") {
		tryNumber(message, height, 0);
		return true;
	}


	else if(message.getAddress() == oscRootAddress + "/center") {
		tryBool(message, bDrawFromCenter, 0);
		return true;
	}

	return false;
}

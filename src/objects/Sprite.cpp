/*==============================================================================

	Visual: a simple, osc-controlled graphics & scripting engine
  
	Copyright (c) 2013 Dan Wilcox <danomatika@gmail.com>

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program. If not, see <http://www.gnu.org/licenses/>.
	
	See https://github.com/danomatika/Visual for documentation

==============================================================================*/
#include "Sprite.h"

//--------------------------------------------------------------
Sprite::Sprite(string name) :
	DrawableObject(name), pos(0, 0), width(0), height(0),
	bAnimate(true), bLoop(true), bPingPong(true),
	bDrawFromCenter(false), bDrawAllLayers(false),
	currentFrame(0), timestamp(0), bForward(true) {
	timestamp = ofGetElapsedTimeMillis();
}

//--------------------------------------------------------------
Sprite::~Sprite() {
	clear();
}

//--------------------------------------------------------------
void Sprite::addFrame(DrawableFrame* frame) {
	if(frame == NULL) {
		ofLogError() << "Sprite \"" << name << "\": cannot add NULL frame";
		return;
	}

	// apply any settings to the added frame
	if(width != 0 && height != 0) {
		frame->setSize(width, height);
	}
	frame->setDrawFromCenter(bDrawFromCenter);
	
	frames.push_back(frame);
	ofLogVerbose(PACKAGE) << "Sprite \"" << name << "\": added " << frame->getType();
}

//--------------------------------------------------------------
void Sprite::removeFrame(DrawableFrame* frame) {
	if(frame == NULL) {
		ofLogError() << "Sprite \"" << name << "\": cannot remove NULL frame";
		return;
	}

	vector<DrawableFrame*>::iterator iter;
	iter = find(frames.begin(), frames.end(), frame);
	if(iter != frames.end()) {
		delete (*iter);
		frames.erase(iter);
	}
}

//--------------------------------------------------------------
void Sprite::clearFrames() {
	/// delete all frames
	for(unsigned int i = 0; i < frames.size(); ++i) {
		DrawableFrame* f = frames.at(i);
		delete f;
	}
	frames.clear();
}

//--------------------------------------------------------------
void Sprite::nextFrame() {
	if(frames.size() < 2) {
		return;
	}

	currentFrame++;
	if(currentFrame >= (int) frames.size()) {
		if(bPingPong) {
			bForward = false;
			currentFrame = frames.size()-2;
		}
		else {
			currentFrame = 0;
		}
	}
}

//--------------------------------------------------------------
void Sprite::prevFrame() {
	if(frames.size() < 2) {
		return;
	}

	currentFrame--;
	if(currentFrame < 0) {
		if(bPingPong) {
			bForward = true;
			currentFrame = 1;
		}
		else {
			currentFrame = frames.size()-1;
		}
	}
}

//--------------------------------------------------------------
void Sprite::gotoFrame(unsigned int num) {
	if(currentFrame >= (int) frames.size()) {
		ofLogWarning() << "Sprite \"" << name << "\": cannot goto frame num " << num
			<< ", index out of range" << endl;
		return;
	}
	currentFrame = num;
}

//--------------------------------------------------------------
void Sprite::gotoFrame(string name) {
	for(unsigned int i = 0; i < frames.size(); ++i) {
		if(name == frames.at(i)->getName()) {
			currentFrame = i;
			return;
		}
	}
}

//--------------------------------------------------------------
void Sprite::setup() {
	for(unsigned int i = 0; i < frames.size(); ++i) {
		frames[i]->setup();
		if(width != 0 && height != 0) {
			frames[i]->setSize(width, height);
		}
	}
}

//--------------------------------------------------------------
void Sprite::draw() {
	if(frames.empty()) {
		return;
	}

	// animate frames?
	if(bAnimate) {
	
		// go to next frame if time has elapsed
		if(ofGetElapsedTimeMillis() - timestamp > frames.at(currentFrame)->getFrameTime()) {
			if(bForward) {
				nextFrame();
			}
			else {
				prevFrame();
			}
			timestamp = ofGetElapsedTimeMillis();
		}
	}

	// draw frame(s)?
	if(bVisible) {
		if(bDrawAllLayers) {
			for(unsigned int i = 0; i < frames.size(); ++i) {
				DrawableFrame* f = frames.at(i);
				f->draw(pos.x, pos.y);
			}
		}
		else if(currentFrame >= 0 && currentFrame < (int) frames.size()) {
			DrawableFrame* f = frames.at(currentFrame);
			f->draw(pos.x, pos.y);
		}
	}
}

//--------------------------------------------------------------
void Sprite::setSize(unsigned int w, unsigned int h) {
	width = w;
	height = h;
	for(unsigned int i = 0; i < frames.size(); ++i) {
		DrawableFrame* f = frames.at(i);
		f->setSize(w, h);
	}
}

//--------------------------------------------------------------
void Sprite::setWidth(unsigned int w) {
	for(unsigned int i = 0; i < frames.size(); ++i) {
		DrawableFrame* f = frames.at(i);
		f->setWidth(w);
	}
}

//--------------------------------------------------------------
void Sprite::setHeight(unsigned int h) {
	for(unsigned int i = 0; i < frames.size(); ++i) {
		DrawableFrame* f = frames.at(i);
		f->setHeight(h);
	}
}

//--------------------------------------------------------------
void Sprite::setAnimation(bool animate, bool loop, bool pingPong) {
	bAnimate = animate;
	bLoop = loop;
	bPingPong = pingPong;
}

//--------------------------------------------------------------
void Sprite::setDrawFromCenter(bool yesno) {
	bDrawFromCenter = yesno;
	for(unsigned int i = 0; i < frames.size(); ++i) {
		DrawableFrame* f = frames.at(i);
		f->setDrawFromCenter(bDrawFromCenter);
	}
}

// PROTECTED
//--------------------------------------------------------------
void Sprite::resizeIfNecessary() {
	if(width != 0 && height != 0) {
		for(unsigned int i = 0; i < frames.size(); ++i) {
			DrawableFrame* f = frames.at(i);
			f->setSize(width, height);
		}
	}
}

//--------------------------------------------------------------
bool Sprite::processOscMessage(const ofxOscMessage& message) {
	
	// call the base class
	if(DrawableObject::processOscMessage(message)) {
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
		resizeIfNecessary();
		return true;
	}
	else if(message.getAddress() == oscRootAddress + "/size/width") {
		tryNumber(message, width, 0);
		resizeIfNecessary();
		return true;
	}
	else if(message.getAddress() == oscRootAddress + "/size/height") {
		tryNumber(message, height, 0);
		resizeIfNecessary();
		return true;
	}

	else if(message.getAddress() == oscRootAddress + "/frame") {
		unsigned int frame = 0;
		tryNumber(message, frame, 0);
		gotoFrame(frame);
		return true;
	}


	else if(message.getAddress() == oscRootAddress + "/animate") {
		tryBool(message, bAnimate, 0);
		return true;
	}
	else if(message.getAddress() == oscRootAddress + "/loop") {
		tryBool(message, bLoop, 0);
		return true;
	}
	else if(message.getAddress() == oscRootAddress + "/pingpong") {
		tryBool(message, bPingPong, 0);
		return true;
	}
	
	else if(message.getAddress() == oscRootAddress + "/center") {
		bool b = bDrawFromCenter;
		if(tryBool(message, b, 0)) {
			setDrawFromCenter(b);
		}
		return true;
	}
	else if(message.getAddress() == oscRootAddress + "/overlay") {
		tryBool(message, bDrawAllLayers, 0);
		return true;
	}

	return false;
}

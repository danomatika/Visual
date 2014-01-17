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
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
	
	See https://github.com/danomatika/Visual for documentation

==============================================================================*/
#include "Image.h"

//--------------------------------------------------------------
Image::Image(string name) : DrawableFrame(name),
	pos(0, 0), width(0), height(0), bDrawFromCenter(false) {
	clear();
}

//--------------------------------------------------------------
Image::Image(string name, string filename) : DrawableFrame(name),
	pos(0, 0), width(0), height(0), bDrawFromCenter(false), filename(filename) {
	loadFile(filename);
	color.set(255);
}

//--------------------------------------------------------------
Image::Image(unsigned int frameTime, string filename) : DrawableFrame("", frameTime),
	pos(0, 0), width(0), height(0), bDrawFromCenter(false), filename(filename) {
	loadFile(filename);
	color.set(255);
}

//--------------------------------------------------------------
bool Image::loadFile(string filename) {
	if(filename == "") {
		filename = this->filename;
	}
	
	bool loaded = false;
	if(!Config::instance().resourceManager.imageExists(filename)) {
		if(!Config::instance().resourceManager.addImage(filename, filename)) {
			ofLogWarning() << "Image: \"" << name << "\" couldn't load \""
				<< filename << "\"";
			return false;
		}
		loaded = true;
		this->filename = filename;
	}
	image = Config::instance().resourceManager.getImage(filename);

	if(loaded) {
		ofLogVerbose(PACKAGE) << "Image: loaded \"" << filename << "\" "
				<< image->getWidth() << "x" << image->getHeight();
	}
	
	// get dimes from image if not set
	if(width == 0)	width = image->getWidth();
	if(height == 0) height = image->getHeight();

	return true;
}

//--------------------------------------------------------------
void Image::setup() {
	if(Config::instance().resourceManager.imageExists(filename)) {
		image = Config::instance().resourceManager.getImage(filename);
	}
	else {
		loadFile();
	}
}

//--------------------------------------------------------------
void Image::draw() {
	draw(pos.x, pos.y, width, height);
}

//--------------------------------------------------------------
void Image::draw(int x, int y) {
	draw(x, y, width, height);
}

//--------------------------------------------------------------
void Image::draw(int x, int y, unsigned int w, unsigned int h) {
	if(image.get() == NULL || !image->isAllocated() || !bVisible) {
		return;
	}

	ofSetColor(color);
	if(bDrawFromCenter) {
		image->draw(x-w/2, y-h/2, w, h);
	}
	else {
		image->draw(x, y, w, h);
	}
}

//--------------------------------------------------------------
void Image::clear() {
	image = ofPtr<ofImage>(); // NULL
	color.set(255);
}

//--------------------------------------------------------------
void Image::setSize(unsigned int w, unsigned int h) {
	width = w;
	height = h;
}

//--------------------------------------------------------------
bool Image::processOscMessage(const ofxOscMessage& message) {

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

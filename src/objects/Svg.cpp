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
#include "Svg.h"

//--------------------------------------------------------------
Svg::Svg(string name) : DrawableFrame(name), bLoaded(false),
	pos(0, 0), width(0), height(0), bDrawFromCenter(false) {
	clear();
}

//--------------------------------------------------------------
Svg::Svg(string name, string filename) : DrawableFrame(name), bLoaded(false),
	pos(0, 0), width(0), height(0), bDrawFromCenter(false), filename(filename) {
	clear();
}

//--------------------------------------------------------------
Svg::Svg(unsigned int frameTime, string filename) :
	DrawableFrame("", frameTime), bLoaded(false),
	pos(0, 0), width(0), height(0), bDrawFromCenter(false), filename(filename) {
	clear();
}

//--------------------------------------------------------------
bool Svg::loadFile(string filename) {
	if(filename == "") {
		filename = this->filename;
	}
	
	bLoaded = false;
	string baseName = ofFilePath::getBaseName(filename);
	if(!Config::instance().resourceManager.svgExists(baseName)) {
		if(!Config::instance().resourceManager.addSvg(baseName, filename)) {
			ofLogWarning() << "Svg: \"" << name << "\" couldn't load \""
				<< filename << "\"";
			return false;
		}
		bLoaded = true;
		this->filename = filename;
	}
	svg = Config::instance().resourceManager.getSvg(baseName);

	if(bLoaded) {
		ofLogVerbose(PACKAGE) << "Svg: loaded \"" << baseName << "\" "
				<< svg->getWidth() << "x" << svg->getHeight() << " "
				<< svg->getNumPath() << (svg->getNumPath() > 1 ? " paths" : " path");
	}
	
	// get dimen from svg if not set
	if(width == 0)	width = svg->getWidth();
	if(height == 0) height = svg->getHeight();

	return true;
}

//--------------------------------------------------------------
void Svg::setup() {
	string baseName = ofFilePath::getBaseName(filename);
	if(Config::instance().resourceManager.svgExists(baseName)) {
		svg = Config::instance().resourceManager.getSvg(baseName);
		if(width == 0)	width = svg->getWidth();
		if(height == 0) height = svg->getHeight();
		bLoaded = true;
	}
	else {
		loadFile();
	}
}

//--------------------------------------------------------------
void Svg::draw() {
	draw(pos.x, pos.y, width, height);
}

//--------------------------------------------------------------
void Svg::draw(int x, int y) {
	draw(x, y, width, height);
}

//--------------------------------------------------------------
void Svg::draw(int x, int y, unsigned int w, unsigned int h) {
	if(!bLoaded || !bVisible) {
		return;
	}
	
	ofSetColor(color);
	if(bDrawFromCenter) {
		ofPushMatrix();
			float scaleX = w/svg->getWidth(), scaleY = h/svg->getHeight();
			ofTranslate(x - (svg->getWidth()/2 * scaleX), y - (svg->getHeight()/2 * scaleY));
			ofScale(scaleX, scaleY, 1);
			svg->draw();
		ofPopMatrix();
	}
	else {
		ofPushMatrix();
			ofTranslate(x, y);
			ofScale(w/svg->getWidth(), h/svg->getHeight(), 1);
			svg->draw();
		ofPopMatrix();
	}
}

//--------------------------------------------------------------
void Svg::clear() {
	svg = ofPtr<ofxSVG>(new ofxSVG); // empty
	color.set(255);
}

//--------------------------------------------------------------
void Svg::setSize(unsigned int w, unsigned int h) {
	width = w;
	height = h;
}

//--------------------------------------------------------------
bool Svg::processOscMessage(const ofxOscMessage& message) {

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

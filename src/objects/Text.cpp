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
#include "Text.h"

//--------------------------------------------------------------
Text::Text(string name) : DrawableObject(name),
	fontSize(DEFAULT_FONT_SIZE), bDrawFromCenter(false) {
	clear();
	fontFilename = Config::instance().fontFilename;
}

//--------------------------------------------------------------
Text::Text(string name, string filename) : DrawableObject(name),
	fontFilename(filename), fontSize(DEFAULT_FONT_SIZE), bDrawFromCenter(false) {
	clear();
	if(this->fontFilename == "") {
		this->fontFilename = Config::instance().fontFilename;
	}
}

//--------------------------------------------------------------
Text::Text(string name, unsigned int size) :
	DrawableObject(name), fontSize(size), bDrawFromCenter(false) {
	clear();
	fontFilename = Config::instance().fontFilename;
}

//--------------------------------------------------------------
Text::Text(string name, string filename, unsigned int size) :
	DrawableObject(name), fontFilename(filename), fontSize(size), bDrawFromCenter(false) {
	clear();
	if(fontFilename == "") {
		fontFilename = Config::instance().fontFilename;
	}
}

//--------------------------------------------------------------
bool Text::loadFont(string filename, unsigned int size) {
	if(filename == "") {
		filename = fontFilename;
	}
	
	bool loaded = false;
	string baseName = ofFilePath::getBaseName(filename);
	if(!Config::instance().resourceManager.fontExists(baseName, fontSize)) {
		if(!Config::instance().resourceManager.addFont(baseName, fontSize, ofToDataPath(filename))) {
			ofLogWarning() << "Text: \"" << name << "\" couldn't load \"" << filename << "\"";
			return false;
		}
		fontFilename = filename;
		loaded = true;
	}
	font = Config::instance().resourceManager.getFont(baseName, fontSize);

	if(loaded) {
		ofLogVerbose(PACKAGE) << "Text: loaded \"" << baseName << "\" " << fontSize;
	}

	return true;
}

//--------------------------------------------------------------
void Text::setup() {
	string baseName = ofFilePath::getBaseName(fontFilename);
	if(Config::instance().resourceManager.fontExists(baseName, fontSize)) {
		font = Config::instance().resourceManager.getFont(baseName, fontSize);
	}
	else {
		loadFont("", fontSize);
	}
}

//--------------------------------------------------------------
void Text::clear() {
	font = ofPtr<ofTrueTypeFont>(new ofTrueTypeFont); // empty font
	color.set(255);
}

//--------------------------------------------------------------
void Text::draw() {
	if(!bVisible && !text.empty()) {
		return;
	}
	ofSetColor(color);
	if(bDrawFromCenter) {
		int w = font->stringWidth(text);
		int h = font->stringHeight(text);
		font->drawString(text, pos.x-w/2, pos.y-h/2);
	}
	else {
		font->drawString(text, pos.x, pos.y);
	}
}

//--------------------------------------------------------------
void Text::draw(int x, int y) {
	if(!bVisible && !text.empty()) {
		return;
	}
	ofSetColor(color);
	if(bDrawFromCenter) {
		int w = font->stringWidth(text);
		int h = font->stringHeight(text);
		font->drawString(text, x-w/2, y-h/2);
	}
	else {
		font->drawString(text, x, y);
	}
}

// PROTECTED
//--------------------------------------------------------------
bool Text::processOscMessage(const ofxOscMessage& message) {

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


	else if(message.getAddress() == oscRootAddress + "/text") {
		tryString(message, text, 0);
		return true;
	}
	
	else if(message.getAddress() == oscRootAddress + "/center") {
		tryBool(message, bDrawFromCenter, 0);
		return true;
	}


	return false;
}

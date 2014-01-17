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
#include "Bitmap.h"

//--------------------------------------------------------------
Bitmap::Bitmap(string name, unsigned int w, unsigned int h) :
	DrawableFrame(name), bitmapWidth(w), bitmapHeight(h),
	pos(0, 0), width(1), height(1),
	bFilled(true), bDrawFromCenter(false) {
	clear();
}

//--------------------------------------------------------------
Bitmap::Bitmap(unsigned int frameTime, unsigned int w, unsigned int h) :
	DrawableFrame("", frameTime), bitmapWidth(w), bitmapHeight(h),
	pos(0, 0), width(1), height(1),
	bFilled(true), bDrawFromCenter(false) {
	clear();
}

//--------------------------------------------------------------
void Bitmap::draw() {
	draw(pos.x, pos.y);
}

//--------------------------------------------------------------
void Bitmap::draw(int x, int y) {
	if(bitmap.empty() || !bVisible) {
		return;
	}

	int xPos = x, yPos = y;

	if(bDrawFromCenter) {
		xPos = xPos - width/2;
		yPos = yPos - height/2;
	}

	ofSetColor(color);
	
	if(bFilled) {
		ofFill();
	}
	else {
		ofNoFill();
	}
	
	ofSetRectMode(OF_RECTMODE_CORNER);
	
	for(unsigned int _y = 0; _y < bitmapHeight; ++_y) {
		for(unsigned int _x = 0; _x < bitmapWidth; ++_x) {
			if(bitmap.at(_y*bitmapWidth + _x)) {
				ofRect(xPos, yPos, pixelWidth, pixelHeight);
			}
			xPos += pixelWidth;
		}
		
		if(bDrawFromCenter) {
			xPos = x - width/2;
		}
		else {
			xPos = x;
		}
		yPos += pixelHeight;
	}
}

//--------------------------------------------------------------
// same as above but computers pixel sizes based on given dimens
void Bitmap::draw(int x, int y, unsigned int w, unsigned int h) {
	if(bitmap.empty() || !bVisible) {
		return;
	}

	int xPos = x, yPos = y;
	int pixWidth = bitmapWidth/w;
	int pixHeight = bitmapHeight/h;

	if(bDrawFromCenter) {
		xPos = xPos - width/2;
		yPos = yPos - height/2;
	}

	ofSetColor(color);
	
	if(bFilled) {
		ofFill();
	}
	else {
		ofNoFill();
	}
	
	ofSetRectMode(OF_RECTMODE_CORNER);
	
	for(unsigned int _y = 0; _y < bitmapHeight; ++_y) {
		for(unsigned int _x = 0; _x < bitmapWidth; ++_x) {
			if(bitmap.at(_y*bitmapWidth + _x)) {
				ofRect(xPos, yPos, pixWidth, pixHeight);
			}
			xPos += pixWidth;
		}
		
		if(bDrawFromCenter) {
			xPos = x - w/2;
		}
		else {
			xPos = x;
		}
		yPos += pixHeight;
	}
}

//--------------------------------------------------------------
void Bitmap::clear() {
	bitmap.clear();
	pixelWidth = 1;
	pixelHeight = 1;
}

//--------------------------------------------------------------
void Bitmap::setBitmap(string bitmapString) {

	stringstream frame(bitmapString);
	string line;
	unsigned int numPix = 0;

	// read through all the chars
	while(!frame.eof()) {
		frame >> line;
		ofLogVerbose(PACKAGE) << "    " << line;

		stringstream chars(line);

		char c;
		chars >> c;
		while(!chars.eof()) {
			switch(c) {
				case '*':   // filled
					bitmap.push_back(true);
					numPix++;
					break;
				case '-':   // empty
					bitmap.push_back(false);
					numPix++;
					break;
				default: // ignore
					break;
			}
			chars >> c;
		}
	}

	// correct size?
	if(bitmapWidth == 0)	bitmapWidth = 1;
	if(bitmapHeight == 0)	bitmapHeight = 1;
	if(numPix < bitmapWidth*bitmapHeight) {
		ofLogWarning() << "Bitmap: Not enough pixels in frame: " << numPix
			<< ", need " << bitmapWidth*bitmapHeight << std::endl;
		bitmap.clear();
		return false;
	}
	
	computePixelSize();
}

//--------------------------------------------------------------
string Bitmap::getBitmap() {
	
	string bitmapString = "";
	
	for(unsigned int _y = 0; _y < bitmapHeight; ++_y) {
		for(unsigned int _x = 0; _x < bitmapWidth; ++_x) {
			if(bitmap.at(_y*bitmapWidth + _x)) {
				bitmapString += '*';
			}
			else {
				bitmapString += '-';
			}
		}
		bitmapString += '\n';
	}
	
	return bitmapString;
}

//--------------------------------------------------------------
void Bitmap::setWidth(unsigned int w) {
	width = w;
	computePixelSize();
}

//--------------------------------------------------------------
void Bitmap::setHeight(unsigned int h) {
	height = h;
	computePixelSize();
}

//--------------------------------------------------------------
void Bitmap::setSize(unsigned int w, unsigned int h) {
	width = w;
	height = h;
	computePixelSize();
}

// PROTECTED
//--------------------------------------------------------------
void Bitmap::computePixelSize() {
	pixelWidth = width/bitmapWidth;
	pixelHeight = height/bitmapHeight;
}

//--------------------------------------------------------------
bool Bitmap::processOscMessage(const ofxOscMessage& message) {

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
		computePixelSize();
		return true;
	}
	else if(message.getAddress() == oscRootAddress + "/size/width") {
		tryNumber(message, width, 0);
		computePixelSize();
		return true;
	}
	else if(message.getAddress() == oscRootAddress + "/size/height") {
		tryNumber(message, height, 0);
		computePixelSize();
		return true;
	}


	else if(message.getAddress() == oscRootAddress + "/center") {
		tryBool(message, bDrawFromCenter, 0);
		return true;
	}


	return false;
}

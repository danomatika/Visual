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
#pragma once

#include "DrawableObject.h"

class Rectangle : public DrawableObject {

	public:

		Rectangle(string name) :
			DrawableObject(name), pos(0, 0),
			width(1), height(1), bFilled(true), bDrawFromCenter(false) {}
		
		Rectangle(string name, int x, int y, unsigned int w, unsigned int h) :
			DrawableObject(name), pos(x, y), width(w), height(h), bFilled(true),
			bDrawFromCenter(false) {}
			
		Rectangle(string name, ofPoint &p, unsigned int w, unsigned int h) :
			DrawableObject(name), pos(p), width(w), height(h), bFilled(true),
			bDrawFromCenter(false) {}

		void draw() {
		
			if(bVisible) {
				ofSetColor(color);

				if(bFilled) {
					ofFill();
				}
				else {
					ofNoFill();
				}
					
				if(bDrawFromCenter) {
					ofSetRectMode(OF_RECTMODE_CENTER);
				}
				else {
					ofSetRectMode(OF_RECTMODE_CORNER);
				}

				ofDrawRectangle(pos, width, height);
			}
		}
		
		// getters / setters
		ofPoint& getPos() {return pos;}
		void setPos(ofPoint &p) {pos = p;}
		
		void setSize(unsigned int w, unsigned int h) {
			width = w;
			height = h;
		}
		unsigned int getWidth() {return width;}
		void setWidth(unsigned int w) {width = w;}
		unsigned int getHeight() {return height;}
		void setHeight(unsigned int h) {height = h;}
		
		bool getFilled() {return bFilled;}
		void setFilled(bool f) {bFilled = f;}
		
		bool getDrawFromCenter() {return bDrawFromCenter;}
		void setDrawFromCenter(bool c) {bDrawFromCenter = c;}
		
		string getType() {return "rectangle";}

	protected:

		/// osc callback
		bool processOscMessage(const ofxOscMessage& message) {
		
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


			else if(message.getAddress() == oscRootAddress + "/filled") {
				tryBool(message, bFilled, 0);
				return true;
			}
			
			else if(message.getAddress() == oscRootAddress + "/center") {
				tryBool(message, bDrawFromCenter, 0);
				return true;
			}


			return false;
		}

		ofPoint pos;
		unsigned int width, height;
		bool bFilled;
		bool bDrawFromCenter; //< draw from the center using pos
};

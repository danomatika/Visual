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
#pragma once

//#include "Config.h"

#include "DrawableObject.h"

class Pixel : public DrawableObject {

	public:

		Pixel(string name) :
			DrawableObject(name), pos(0, 0) {}
		
		Pixel(string name, int x, int y) : DrawableObject(name), pos(x, y) {}

		void draw() {
			if(bVisible) {
				ofSetColor(color);
				ofFill();
				ofRect(pos, 1, 1);
			}
		}
		
		// getters / setters
		ofPoint& getPos() {return pos;}
		void setPos(ofPoint &p) {pos = p;}
		
		string getType() {return "pixel";}

	protected:

		/// osc callback
		bool processOscMessage(const ofxOscMessage& message) {
		
			// call the base class
			if(DrawableObject::processOscMessage(message)) {
				return true;
			}


			else if(message.getAddress() == oscRootAddress + "/position") {
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


			return false;
		}

		ofPoint pos;
};

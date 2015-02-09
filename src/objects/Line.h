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
#pragma once

#include "DrawableObject.h"

class Line : public DrawableObject {

	public:

		Line(string name) :
			DrawableObject(name), pos1(0, 0), pos2(0, 0) {}
		
		Line(string name, int x1, int y1, int x2, int y2) : DrawableObject(name),
			pos1(x1, y1), pos2(x2, x2) {}
			
		Line(string name, ofPoint& p1, ofPoint& p2) : DrawableObject(name),
			pos1(p1), pos2(p2) {}

		void draw() {
			if(bVisible) {
				ofSetColor(color);
				ofLine(pos1, pos2);
			}
		}
		
		// getters / setters
		ofPoint& getPos1() {return pos1;}
		void setPos1(ofPoint &p) {pos1 = p;}
		
		ofPoint& getPos2() {return pos2;}
		void setPos2(ofPoint &p) {pos2 = p;}
		
		string getType() {return "line";}

	protected:

		/// osc callback
		bool processOscMessage(const ofxOscMessage& message) {
		
			// call the base class
			if(DrawableObject::processOscMessage(message)) {
				return true;
			}


			if(message.getAddress() == oscRootAddress + "/position1") {
				tryNumber(message, pos1.x, 0);
				tryNumber(message, pos1.y, 1);
				return true;
			}
			else if(message.getAddress() == oscRootAddress + "/position1/x") {
				tryNumber(message, pos1.x, 0);
				return true;
			}
			else if(message.getAddress() == oscRootAddress + "/position1/y") {
				tryNumber(message, pos1.y, 0);
				return true;
			}


			else if(message.getAddress() == oscRootAddress + "/position2") {
				tryNumber(message, pos2.x, 0);
				tryNumber(message, pos2.y, 1);
				return true;
			}
			else if(message.getAddress() == oscRootAddress + "/position2/x") {
				tryNumber(message, pos2.x, 0);
				return true;
			}
			else if(message.getAddress() == oscRootAddress + "/position2/y") {
				tryNumber(message, pos2.y, 0);
				return true;
			}


			return false;
		}

		ofPoint pos1, pos2;
};

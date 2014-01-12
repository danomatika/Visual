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

class Line : public DrawableObject {

	public:

		Line(string name, string parentOscAddress) :
			DrawableObject("line", name, parentOscAddress), pos1(0, 0), pos2(0, 0) {
//			// add variables to Xml
//			addXmlAttribute("x", "position1", XML_TYPE_FLOAT, &pos1.x);
//			addXmlAttribute("y", "position1", XML_TYPE_FLOAT, &pos1.y);
//			addXmlAttribute("x", "position2", XML_TYPE_FLOAT, &pos2.x);
//			addXmlAttribute("y", "position2", XML_TYPE_FLOAT, &pos2.y);
		}

		void draw() {
			if(bVisible) {
				ofSetColor(color);
				ofLine(pos1, pos2);
			}
		}
		
		string getType() {return "line";}

	protected:

		bool processOscMessage(const ofxOscMessage& message) {
		
			// call the base class
			if(DrawableObject::processOscMessage(message, source)) {
				return true;
			}


			if(message.getAddress() == oscRootAddress + "/position1") {
				Util::tryNumber(message, pos1.x, 0);
				Util::tryNumber(message, pos1.y, 1);
				return true;
			}
			else if(message.getAddress() == oscRootAddress + "/position1/x") {
				Util::tryNumber(message, pos1.x, 0);
				return true;
			}
			else if(message.getAddress() == oscRootAddress + "/position1/y") {
				Util::tryNumber(message, pos1.y, 0);
				return true;
			}


			else if(message.getAddress() == oscRootAddress + "/position2") {
				Util::tryNumber(message, pos2.x, 0);
				Util::tryNumber(message, pos2.y, 1);
				return true;
			}
			else if(message.getAddress() == oscRootAddress + "/position2/x") {
				Util::tryNumber(message, pos2.x, 0);
				return true;
			}
			else if(message.getAddress() == oscRootAddress + "/position2/y") {
				Util::tryNumber(message, pos2.y, 0);
				return true;
			}


			return false;
		}

		ofPoint pos1, pos2;
};

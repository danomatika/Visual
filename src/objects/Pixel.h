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

		Pixel(string name, string parentOscAddress) :
			DrawableObject("pixel", name, parentOscAddress), pos(0, 0) {
			// add variables to Xml
			addXmlAttribute("x", "position", XML_TYPE_FLOAT, &pos.x);
			addXmlAttribute("y", "position", XML_TYPE_FLOAT, &pos.y);
		}

		void draw() {
			if(bVisible) {
				visual::Graphics::stroke(color);
				visual::Graphics::point(pos.x, pos.y);
			}
		}
		
		string getType() {return "pixel";}

	protected:

		bool processOscMessage(const ofxOscMessage& message) {
			// call the base class
			if(DrawableObject::processOscMessage(message, source)) {
				return true;
			}


			else if(message.getAddress() == oscRootAddress + "/position") {
				Util::tryNumber(message, pos.x, 0);
				Util::tryNumber(message, pos.y, 1);
				return true;
			}
			else if(message.getAddress() == oscRootAddress + "/position/x") {
				Util::tryNumber(message, pos.x, 0);
				return true;
			}
			else if(message.getAddress() == oscRootAddress + "/position/y") {
				Util::tryNumber(message, pos.y, 0);
				return true;
			}


			return false;
		}

		ofPoint pos;
};

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

#include "OscObject.h"
#include "../Config.h"
#include "../Util.h"

class DrawableObject : public OscObject {

	public:

		DrawableObject(string objectType, string objectName, string parentOscAddress) :
//			XmlObject(objectType),
			OscObject(parentOscAddress+"/"+objectName),
			bVisible(true), name(objectName) {
			
//			// attach variables to Xml
//			addXmlAttribute("name", objectType, XML_TYPE_STRING, &name);
//			
//			addXmlAttribute("R", "color", XML_TYPE_BYTE, &color.R);
//			addXmlAttribute("G", "color", XML_TYPE_BYTE, &color.G);
//			addXmlAttribute("B", "color", XML_TYPE_BYTE, &color.B);
//			addXmlAttribute("A", "color", XML_TYPE_BYTE, &color.A);
//
//			addXmlAttribute("yesno", "visible", XML_TYPE_BOOL, &bVisible);
			
			ofLog() << "	OBJECT " << parentOscAddress+"/"+objectName << endl;
		}
		virtual ~DrawableObject() {}

		virtual void setup() {};
		virtual void draw() = 0;
		virtual void draw(int x, int y) {}
		
		virtual void setSize(unsigned int width, unsigned int height) {}
		virtual void setDrawFromCenter(bool yesno) {}

		string getName() {return name;}
		virtual string getType() = 0;

	protected:

		/// process one osc message, derived objects should call this
		/// using DrawableObject::processOscMessage() to handle the base variables
		virtual bool processOscMessage(const ofxOscMessage& message) {
									   
			if(message.getAddress() == oscRootAddress + "/color") {
				if(message.getNumArgs() > 2) {
					Util::tryNumber(message, (unsigned int&) color.r, 0);
					Util::tryNumber(message, (unsigned int&) color.g, 1);
					Util::tryNumber(message, (unsigned int&) color.b, 2);
					if(message.getNumArgs() > 3) {
						Util::tryNumber(message, (unsigned int&) color.a, 3);
					}
				}
				return true;
			}
			else if(message.getAddress() == oscRootAddress + "/color/R") {
				Util::tryNumber(message, (unsigned int&) color.r, 0);
				return true;
			}
			else if(message.getAddress() == oscRootAddress + "/color/G") {
				Util::tryNumber(message, (unsigned int&) color.g, 0);
				return true;
			}
			else if(message.getAddress() == oscRootAddress + "/color/B") {
				Util::tryNumber(message, (unsigned int&) color.b, 0);
				return true;
			}
			else if(message.getAddress() == oscRootAddress + "/color/A") {
				Util::tryNumber(message, (unsigned int&) color.a, 0);
				return true;
			}

			else if(message.getAddress() == oscRootAddress + "/visible") {
				Util::tryBool(message, bVisible, 0);
				return true;
			}

			return false;
		}

		ofColor color;
		bool bVisible;
		string name;
};

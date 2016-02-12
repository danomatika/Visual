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

#include "OscObject.h"
#include "../Config.h"

class DrawableObject : public OscObject {

	public:

		DrawableObject(string objectName) : bVisible(true), name(objectName) {}
		virtual ~DrawableObject() {}

		virtual void setup() {}
		virtual void update() {}
		virtual void draw() {}
		virtual void draw(int x, int y) {}
		virtual void draw(int x, int y, unsigned int w, unsigned h) {}
		virtual void clear() {}
		
		// for children
		virtual void setSize(unsigned int w, unsigned int h) {}
		virtual void setWidth(unsigned int w) {}
		virtual void setHeight(unsigned int h) {}
		virtual void setDrawFromCenter(bool yesno) {}

		// getters / setters
		ofColor& getColor() {return color;}
		void setColor(ofColor& c) {color = c;}
		
		bool getVisible() {return bVisible;}
		void setVisible(bool v) {bVisible = v;}
		
		string getName() {return name;}
		
		virtual string getType() {return "drawableobject";}
		
		/// can this object be setup before it's parent scene is active?
		virtual bool allowsEarlySetup() {return true;}
		
		/// should this object always be setup when it's parent scene becomes active?
		virtual bool shouldAlwaysBeSetup() {return false;}
		
		/// should this object be cleared when it's parent scene is exiting?
		virtual bool shouldClearOnExit() {return false;}

	protected:

		/// process one osc message, derived objects should call this and call
		/// DrawableObject::processOscMessage() to handle the base variables
		virtual bool processOscMessage(const ofxOscMessage& message) {
									   
			if(message.getAddress() == oscRootAddress + "/color") {
				if(message.getNumArgs() > 2) {
					tryNumber(message, (unsigned int&) color.r, 0);
					tryNumber(message, (unsigned int&) color.g, 1);
					tryNumber(message, (unsigned int&) color.b, 2);
					if(message.getNumArgs() > 3) {
						tryNumber(message, (unsigned int&) color.a, 3);
					}
				}
				return true;
			}
			else if(message.getAddress() == oscRootAddress + "/color/R") {
				tryNumber(message, (unsigned int&) color.r, 0);
				return true;
			}
			else if(message.getAddress() == oscRootAddress + "/color/G") {
				tryNumber(message, (unsigned int&) color.g, 0);
				return true;
			}
			else if(message.getAddress() == oscRootAddress + "/color/B") {
				tryNumber(message, (unsigned int&) color.b, 0);
				return true;
			}
			else if(message.getAddress() == oscRootAddress + "/color/A") {
				tryNumber(message, (unsigned int&) color.a, 0);
				return true;
			}

			else if(message.getAddress() == oscRootAddress + "/visible") {
				tryBool(message, bVisible, 0);
				return true;
			}

			return false;
		}

		ofColor color;
		bool bVisible;
		string name;
};

// drawable object with an animation frame time
class DrawableFrame : public DrawableObject {

	public:
		
		DrawableFrame(string objectName) :
			DrawableObject(objectName), frameTime(0) {}
			
		DrawableFrame(string objectName, unsigned int frameTime) :
			DrawableObject(objectName), frameTime(frameTime) {}
		
		virtual ~DrawableFrame() {}
	
		virtual string getType() {return "drawableframe";}
		
		unsigned int getFrameTime()	{return frameTime;}
		void setFrameTime(unsigned int time) {frameTime = time;}
	
	protected:
	
		unsigned int frameTime;
};

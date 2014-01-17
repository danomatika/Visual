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

#include "DrawableObject.h"

class Text : public DrawableObject {

	public:

		Text(string name) :
			DrawableObject(name), pos(0, 0), bDrawFromCenter(false) {}
		
		Text(string name, string text) :
			DrawableObject(name), pos(0, 0), bDrawFromCenter(false), text(text) {}
		
		Text(string name, string text, int x, int y) :
			DrawableObject(name), pos(x, y), bDrawFromCenter(false), text(text) {}

		Text(string name, string text, ofPoint& p) :
			DrawableObject(name), pos(p), bDrawFromCenter(false), text(text) {}

		void draw() {
			if(bVisible && !text.empty()) {
				ofSetColor(color);
				if(bDrawFromCenter) {
					int w = Config::instance().font.stringWidth(text);
					int h = Config::instance().font.stringHeight(text);
					Config::instance().font.drawString(text, pos.x-w/2, pos.y-h/2);
				}
				else {
					Config::instance().font.drawString(text, pos.x, pos.y);
				}
			}
		}
		
		void draw(int x, int y) {
			if(bVisible && !text.empty()) {
				ofSetColor(color);
				if(bDrawFromCenter) {
					int w = Config::instance().font.stringWidth(text);
					int h = Config::instance().font.stringHeight(text);
					Config::instance().font.drawString(text, x-w/2, y-h/2);
				}
				else {
					Config::instance().font.drawString(text, x, y);
				}
			}
		}
		
		// getters / setters
		ofPoint& getPos() {return pos;}
		void setPos(ofPoint &p) {pos = p;}
		
		string getText() {return text;}
		void setText(string t) {text = t;}
		
		bool getDrawFromCenter() {return bDrawFromCenter;}
		void setDrawFromCenter(bool c) {bDrawFromCenter = c;}
		
		string getType() {return "text";}

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

		ofPoint pos;
		string text;
		bool bDrawFromCenter; ///< draw from the center using pos
};

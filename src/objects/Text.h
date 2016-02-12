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

class Text : public DrawableObject {

	public:

		Text(string name);
		Text(string name, string filename);
		Text(string name, unsigned int size);
		Text(string name, string filename, unsigned int size);

		bool loadFont(string filename="", unsigned int size=0);
		
		void setup();
		void clear();

		void draw();
		void draw(int x, int y);
		
		// getters / setters
		ofTrueTypeFont& getFont() {return *font;}
		bool isFontLoaded() {return font->isLoaded();}
		
		string getFontFilename() {return fontFilename;}
		unsigned int getFontSize() {return fontSize;}
		
		ofPoint& getPos() {return pos;}
		void setPos(ofPoint &p) {pos = p;}
		
		string getText() {return text;}
		void setText(string t) {text = t;}
		
		bool getDrawFromCenter() {return bDrawFromCenter;}
		void setDrawFromCenter(bool c) {bDrawFromCenter = c;}
		
		string getType() {return "text";}

	protected:

		/// osc callback
		bool processOscMessage(const ofxOscMessage& message);

		ofPtr<ofTrueTypeFont> font;
		string fontFilename;
		unsigned int fontSize;
		
		ofPoint pos;
		string text;
		bool bDrawFromCenter; //< draw from the center using pos
};

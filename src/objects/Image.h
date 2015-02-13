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

class Image : public DrawableFrame {

	public:

		Image(string name);
		Image(string name, string filename);
		Image(unsigned int frameTime, string filename);

		bool loadFile(string filename="");

		void setup();

		void draw();
		void draw(int x, int y);
		void draw(int x, int y, unsigned int w, unsigned int h);
		
		void clear();
		
		// getters / setters
		ofImage& getImage() {return *image;}
		bool isLoaded() {return image->isAllocated();}
		
		string getFilename() {return filename;}
		
		ofPoint& getPos() {return pos;}
		void setPos(ofPoint &p) {pos = p;}
		
		void setSize(unsigned int w, unsigned int h);
		unsigned int getWidth() {return width;}
		void setWidth(unsigned int w) {width = w;}
		unsigned int getHeight() {return height;}
		void setHeight(unsigned int h) {height = h;}
		
		bool getDrawFromCenter() {return bDrawFromCenter;}
		void setDrawFromCenter(bool c) {bDrawFromCenter = c;}
		
		string getType() {return "image";}

	protected:

		/// osc callback
		bool processOscMessage(const ofxOscMessage& message);
		
		ofPtr<ofImage> image;

		string filename;
		ofPoint pos;
		unsigned int width, height;
		bool bDrawFromCenter; //< draw from the center using pos
};

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

class Bitmap : public DrawableFrame {

	public:

		Bitmap(string name, unsigned int w, unsigned int h);
		Bitmap(unsigned int frameTime, unsigned int w, unsigned int h);

		void draw();
		void draw(int x, int y);
		void draw(int x, int y, unsigned int w, unsigned int h);
		void clear();

		void setBitmap(vector<bool> bitmap) {this->bitmap = bitmap;}
		
		// getters / setters
		void setBitmap(string bitmapString); // computes from string
		string getBitmap();

		ofPoint& getPos() {return pos;}
		void setPos(ofPoint &p) {pos = p;}
		
		void setSize(unsigned int w, unsigned int h);
		unsigned int getWidth() {return width;}
		void setWidth(unsigned int w);
		unsigned int getHeight() {return height;}
		void setHeight(unsigned int h);
		
		bool getFilled() {return bFilled;}
		void setFilled(bool f) {bFilled = f;}
		
		bool getDrawFromCenter() {return bDrawFromCenter;}
		void setDrawFromCenter(bool c) {bDrawFromCenter = c;}
		
		string getType() {return "bitmap";}

	protected:
	
		/// compute the pixel size based on the width/height and size of bitmap
		void computePixelSize();

		/// osc callback
		bool processOscMessage(const ofxOscMessage& message);

		vector<bool> bitmap; ///< actual bitmap
		unsigned int bitmapWidth, bitmapHeight;	///< dimen of the bitmap

		ofPoint pos; ///< screen position
		unsigned int width, height; ///< screen size
		unsigned int pixelWidth, pixelHeight; ///< dimen of one pixel in the bitmap
		bool bFilled;
		bool bDrawFromCenter; ///< draw from the center using pos?
};

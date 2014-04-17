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

#include "ofMain.h"

// simple console of lines of text,
// using bitmap string for now ...
class Console {

	public:

		Console();

		void setup(); // calculate to fill screen
		void setup(unsigned int width, unsigned int height);

		void resizeToScreen();

		// add a line to the buffer
		void addLine(string line);

		// clear the buffer
		void clear();

		// draw the console using the current position
		void draw();

		unsigned int width;  // chars
		unsigned int height; // lines
		ofPoint pos;

		static void drawString(string text, int x, int y);

	protected:

		typedef vector<string> Line; // a vector of space separated words
		deque<Line> lines;
		
		//static ofTrueTypeFont font;
		
		ofMutex mutex;
};

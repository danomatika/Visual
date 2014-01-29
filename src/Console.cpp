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
#include "Console.h"

#include "Config.h"

//ofTrueTypeFont Console::font;

//--------------------------------------------------------------
Console::Console() {
	width = 0;
	height = 0;
	//pos.set(0, 16);
	pos.set(16, 12);
}

//--------------------------------------------------------------
void Console::setup() {
	clear();
	resizeToScreen();
//	if(!font.isLoaded()) {
//		font.loadFont(Config::instance().fontFilename, 8);
//	}
}

//--------------------------------------------------------------
void Console::setup(unsigned int w, unsigned int h) {
	clear();
	width = w;
	height = h;
//	if(!font.isLoaded()) {
//		font.loadFont(Config::instance().fontFilename, 8);
//	}
}

//--------------------------------------------------------------
void Console::resizeToScreen() {
//	width = (int) (ofGetWidth()/(font.stringWidth("O") + font.getLetterSpacing()*2));
//	height = (int) (ofGetHeight()/(font.stringHeight("O") + font.getLineHeight()));
	width = (int) (ofGetWidth()/10);
	height = (int) (ofGetHeight()/14);
}

//--------------------------------------------------------------
void Console::addLine(string line) {
	mutex.lock();
	
	// break up lines by newline & words
	vector<string> wraps = ofSplitString(line, "\n");
	for(int i = 0; i < wraps.size(); ++i) {
		lines.push_back(ofSplitString(wraps[i], " "));
	}
		
	while(lines.size() > height) {
		lines.pop_front();
	}
	
	mutex.unlock();
}

//--------------------------------------------------------------
void Console::clear() {
	mutex.lock();
	lines.clear();
	mutex.unlock();
}

//--------------------------------------------------------------
void Console::draw() {
	mutex.lock();
	
	string line;
	int x = pos.x, y = pos.y;
	
	// draw each line and word wrap if needed
	deque<Line>::iterator iter = lines.begin();
	for(; iter != lines.end(); ++iter) {
		vector<string> &words = (*iter);
		int i = 0;
		for(; i < words.size(); ++i) {
			if(line.size() + 1 + words[i].size() >= width) {
				drawString(line, x, y);
				y += 14;//font.getLineHeight();
				line = "";
			}
			line = line + words[i] + " ";
		}
		if(!line.empty()) {
			drawString(line, x, y);
			y += 14;//font.getLineHeight();
			line = "";
		}
	}
	
	mutex.unlock();
}

void Console::drawString(string text, int x, int y) {
	ofSetColor(0);
	//font.drawString(text, x+1, y+1);
	ofDrawBitmapStringHighlight(text, x+1, y+1);
	ofSetColor(255);
	//font.drawString(text, x, y);
	ofDrawBitmapStringHighlight(text, x, y);
}

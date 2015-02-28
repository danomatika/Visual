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

class Video : public DrawableObject {

	public:

		Video(string name);
		Video(string name, string filename);

		bool loadFile(string filename="");

		void setup();
		void update();
		
		void draw();
		void draw(int x, int y);
		void draw(int x, int y, unsigned int w, unsigned int h);
		
		void clear();
	
		void nextFrame();
		void prevFrame();
		
		// getters / setters
		ofVideoPlayer& getVideo() {return *video;}
		bool isLoaded() {return video->isLoaded();}
		
		bool getPlay() {return bPlay;}
		void setPlay(bool b);
		float getVolume() {return volume;}
		void setVolume(float v);
		float getSpeed();
		void setSpeed(float s);
		ofLoopType getLoop();
		void setLoop(ofLoopType s);
		
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
		
		string getType() {return "video";}
		
		bool shouldAlwaysBeSetup() {return true;}

	protected:

		/// osc callback
		bool processOscMessage(const ofxOscMessage& message);
		
		ofPtr<ofVideoPlayer> video;

		bool bPlay;
		float volume;
		float speed;
		bool bLoop;
		ofLoopType loopType;

		string filename;
		ofPoint pos;
		unsigned int width, height;
		bool bDrawFromCenter; //< draw from the center using pos
};

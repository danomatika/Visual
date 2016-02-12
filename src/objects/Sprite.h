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
#include "Bitmap.h"
#include "Image.h"

class Sprite : public DrawableObject {

	public:

		Sprite(string name);
		~Sprite();

		void addFrame(DrawableFrame* frame);
		void removeFrame(DrawableFrame* frame);
		void clearFrames();

		void nextFrame();
		void prevFrame();
		void gotoFrame(unsigned int num);
		void gotoFrame(string name);
		unsigned int getCurrentFrame() {return currentFrame;}

		void setup();
		void draw();
		
		// getters / setters
		ofPoint& getPos() {return pos;}
		void setPos(ofPoint &p) {pos = p;}
		
		void setSize(unsigned int w, unsigned int h);
		unsigned int getWidth() {return width;}
		void setWidth(unsigned int w);
		unsigned int getHeight() {return height;}
		void setHeight(unsigned int h);
		
		void setAnimation(bool animate, bool loop, bool pingPong);
		bool getAnimate() {return bAnimate;}
		void setAnimate(bool a) {bAnimate = a;}
		bool getLoop() {return bLoop;}
		void setLoop(bool l) {bLoop = l;}
		bool getPingPong() {return bPingPong;}
		void setPingPong(bool p) {bPingPong = p;}
		
		bool getDrawFromCenter() {return bDrawFromCenter;}
		void setDrawFromCenter(bool yesno);
		
		bool getDrawAllLayers() {return bDrawAllLayers;}
		void setDrawAllLayers(bool yesno) {bDrawAllLayers = yesno;}
		
		string getType() {return "sprite";}

	protected:

		/// resize child frames if height & width are set
		void resizeIfNecessary();

		/// osc callback
		bool processOscMessage(const ofxOscMessage& message);
		
		vector<DrawableFrame*> frames;

		ofPoint pos;
		unsigned int width, height;
		bool bAnimate;
		bool bLoop;
		bool bPingPong;
		bool bDrawFromCenter;
		bool bDrawAllLayers;

		int currentFrame;
		unsigned int timestamp;
		bool bForward;  //< advance frames?
};

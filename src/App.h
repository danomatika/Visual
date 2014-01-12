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

#include "ofxAppUtils.h"

#include "Config.h"
#include "OscReceiver.h"

class App : public ofBaseApp, public OscObject {

	public:
	
		App();
	
		void setup();
		void update();
		void draw();
		void exit();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		bool shiftPressed; //< shift key modifier
		
		bool bDebug;
		bool bRunning;  //< running or paused?
		
		Config &config;
		OscReceiver &receiver;
		ofxOscSender &sender;
		
		ofxSceneManager sceneManager;
		
		unsigned int reloadTimestamp;
		unsigned int saveTimestamp;
		
	protected:
	
		bool processOscMessage(const ofxOscMessage& message);
};

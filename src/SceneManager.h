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

//#include "Config.h"
#include "Scene.h"
#include "ofxTimer.h"

class SceneManager : public OscObject {

	public:

		SceneManager();
		virtual ~SceneManager();

		/// add a scene
		void addScene(Scene* scene);

		/// remove a scene
		void removeScene(Scene* scene);

		/// clears (deletes) all the objects in the list
		void clear(bool keepCurScene=false);

		// scene transport
		void nextScene();
		void prevScene();
		void gotoScene(unsigned int num);
		void gotoScene(string name);

		/// loads graphics resources
		void setup();

		/// draw all the objects in the list
		void draw();
		
		/// show the scene name when changing?
		void showSceneName(bool yesno) {_bShowSceneName = yesno;}
		void toggleSceneName() {_bShowSceneName = !_bShowSceneName;}
		
		/// set the psuedo frameRate calculated via timer
		void setFrameRate(unsigned int rate);

	protected:

		/// set the background and fps from a scene
		void setupScene(Scene* s);

		/// osc callback
		bool processOscMessage(const ofxOscMessage& message);

	private:

		int _currentScene;
		vector<Scene*> _sceneList;
		
		bool _bShowSceneName;
		ofxTimer _sceneNameTimer;
		ofTrueTypeFont _sceneNameFont;
		
		ofxTimer _frameRateTimer;
		unsigned int _frameRate;
		
		bool _bSetBackground;
};

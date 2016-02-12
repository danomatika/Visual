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
#include "Config.h"

#include "ofApp.h"
#include "Scene.h"
#include "Objects.h" // needed for bindings

/// \section Utils
//--------------------------------------------------------------
void print(const string message) {
	ofLogNotice() << message;
}

/// \section Config reatime functions
//--------------------------------------------------------------
void setListeningPort(unsigned int port) {
	Config::instance().setListeningPort(port);
}

//--------------------------------------------------------------
void setSendingIp(string address) {
	Config::instance().setSendingIp(address);
}

//--------------------------------------------------------------
void setSendingPort(unsigned int port) {
	Config::instance().setSendingPort(port);
}

/// \section Config.h
//--------------------------------------------------------------
Config* getConfig() {
	return &Config::instance();
}

//--------------------------------------------------------------
float getRenderWidth() {
	return Config::instance().renderWidth;
}

//--------------------------------------------------------------
float getRenderHeight() {
	return Config::instance().renderHeight;
}

/// \section Scene.h
//--------------------------------------------------------------
void addScene(Scene *scene_disown) {
	Config::instance().app->sceneManager.addScene(scene_disown);
}

//--------------------------------------------------------------
Scene* getCurrentScene() {
	return Config::instance().app->sceneManager.getCurrentScene();
}

//--------------------------------------------------------------
void clearScenes() {
	Config::instance().app->sceneManager.clear();
}

/// \section Helpers
//--------------------------------------------------------------
// add scene with given script
void addScript(string name, string filename) {
	Scene *s = new Scene(name);
	s->addObject(new Script(name, filename));
	Config::instance().app->sceneManager.addScene(s);
}

//--------------------------------------------------------------
// add object to current scene
void addObject(DrawableObject *object_disown) {
	Scene *s = Config::instance().app->sceneManager.getCurrentScene();
	if(!s) {
		s = new Scene("default");
		Config::instance().app->sceneManager.addScene(s);
		Config::instance().app->sceneManager.gotoScene("default");
	}
	s->addObject(object_disown);
}

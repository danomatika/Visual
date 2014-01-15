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
#include "SceneManager.h"

#include "Config.h"

#define SCENE_NAME_MS			1250
#define SCENE_NAME_FONT_SIZE	50

//--------------------------------------------------------------
SceneManager::SceneManager() :
	OscObject(Config::instance().baseAddress),
	_currentScene(-1), _bShowSceneName(true) {}

//--------------------------------------------------------------
SceneManager::~SceneManager() {
	clear();
}

//--------------------------------------------------------------
void SceneManager::addScene(Scene* scene) {
	if(scene == NULL) {
		ofLogError() << "SceneManager: cannot add NULL scene";
		return;
	}
	scene->setOscRootAddress(oscRootAddress+"/"+scene->getName());
	_sceneList.push_back(scene);
}

//--------------------------------------------------------------
void SceneManager::removeScene(Scene* scene) {
	if(scene == NULL) {
		ofLogError() << "SceneManager: cannot remove NULL scene";
		return;
	}

	vector<Scene*>::iterator iter;
	iter = find(_sceneList.begin(), _sceneList.end(), scene);
	if(iter != _sceneList.end()) {
		_sceneList.erase(iter);
	}
}

//--------------------------------------------------------------
void SceneManager::clear(bool keepCurScene) {

	/// delete all the objects
	for(unsigned int i = 0; i < _sceneList.size(); ++i) {
		Scene* o = _sceneList.at(i);
		delete o;
	}
	_sceneList.clear();
	
	if(!keepCurScene) {
		_currentScene = 0;
	}
}

//--------------------------------------------------------------
void SceneManager::nextScene() {
	if(_sceneList.empty()) {
		return;
	}

	_currentScene++;
	if(_currentScene >= (int) _sceneList.size()) {
		_currentScene = 0;
	}

	ofLogVerbose("visual") << "SceneManager: changed to scene \""
		<< _sceneList.at(_currentScene)->getName() << "\"";

	setupScene(_sceneList.at(_currentScene));
}

//--------------------------------------------------------------
void SceneManager::prevScene() {
	if(_sceneList.empty()) {
		return;
	}

	_currentScene--;
	if(_currentScene < 0) {
		_currentScene = _sceneList.size()-1;
	}

	ofLogVerbose("visual") << "SceneManager: changed to scene \""
		<< _sceneList.at(_currentScene)->getName() << "\"";

	setupScene(_sceneList.at(_currentScene));
}

//--------------------------------------------------------------
void SceneManager::gotoScene(unsigned int num) {
	if(_sceneList.empty()) {
		return;
	}

	if(_currentScene >= (int) _sceneList.size()) {
		ofLogWarning() << "SceneManager: cannot goto scene num " << num
			<< ", index out of range";
		return;
	}

	_currentScene = num;

	ofLogVerbose("visual") << "SceneManager: changed scene to \""
			  << _sceneList.at(_currentScene)->getName() << "\"";

	setupScene(_sceneList.at(_currentScene));
}

//--------------------------------------------------------------
void SceneManager::gotoScene(string name) {

	for(unsigned int i = 0; i < _sceneList.size(); ++i) {
	
		if(name == _sceneList.at(i)->getName()) {
			_currentScene = i;
			ofLogVerbose("visual") << "SceneManager: changed scene to \""
					  << _sceneList.at(_currentScene)->getName() << "\"";
			setupScene(_sceneList.at(_currentScene));
			return;
		}
	}
}

//--------------------------------------------------------------
void SceneManager::setup() {
	
	// load scene change font
	if(!_sceneNameFont.isLoaded()) {
		_sceneNameFont.loadFont(Config::instance().fontFilename, SCENE_NAME_FONT_SIZE);
	}
	
	// setup all scenes
	for(unsigned int i = 0; i < _sceneList.size(); ++i) {
		_sceneList.at(i)->setup();
	}

	// try to load the first scene
	if(_currentScene < 0) {
		gotoScene(0);
	}
}

//--------------------------------------------------------------
void SceneManager::draw() {
	if(_currentScene >= 0 && _currentScene < (int) _sceneList.size()) {
		Scene* s = _sceneList.at(_currentScene);
		s->draw();
		
		if(_bShowSceneName && !_sceneNameTimer.alarm()) {
			ofSetHexColor(0xFF00FF);
			_sceneNameFont.drawString(s->getName(),
				0, Config::instance().renderHeight-SCENE_NAME_FONT_SIZE);
		}
	}
}

// PROTECTED
//--------------------------------------------------------------
void SceneManager::setupScene(Scene* s) {
	ofBackground(s->getBackground());
	if(s->getFps() > 0) {
		ofSetFrameRate(s->getFps());
	}
	_sceneNameTimer.setAlarm(SCENE_NAME_MS);
}

//--------------------------------------------------------------
bool SceneManager::processOscMessage(const ofxOscMessage& message) {
	if(_currentScene >= 0 && _currentScene < (int) _sceneList.size()) {
		return _sceneList.at(_currentScene)->processOsc(message);
	}
	return false;
}

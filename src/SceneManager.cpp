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

#define SCENE_CHANGE_MS			500
#define SCENE_NAME_MS			1250
#define SCENE_NAME_FONT_SIZE	36

//--------------------------------------------------------------
SceneManager::SceneManager() : OscObject(""),
	currentScene(-1), bShowSceneName(true),
	bSetBackground(false), bSetupScene(false) {}

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
	scenes.push_back(scene);
}

//--------------------------------------------------------------
void SceneManager::removeScene(Scene* scene) {
	if(scene == NULL) {
		ofLogError() << "SceneManager: cannot remove NULL scene";
		return;
	}

	vector<Scene*>::iterator iter;
	iter = find(scenes.begin(), scenes.end(), scene);
	if(iter != scenes.end()) {
		scenes.erase(iter);
	}
}

//--------------------------------------------------------------
Scene* SceneManager::getCurrentScene() {
	if(currentScene < 0) {
		return NULL;
	}
	return scenes[currentScene];
}

//--------------------------------------------------------------
void SceneManager::clear(bool keepCurScene) {

	/// delete all the objects
	for(unsigned int i = 0; i < scenes.size(); ++i) {
		Scene* o = scenes.at(i);
		delete o;
	}
	scenes.clear();
	
	if(!keepCurScene) {
		currentScene = 0;
	}
}

//--------------------------------------------------------------
void SceneManager::nextScene() {
	if(scenes.empty() || !sceneChangeTimer.alarm()) {
		return;
	}

	exit();
	currentScene++;
	if(currentScene >= (int) scenes.size()) {
		currentScene = 0;
	}

	ofLogVerbose(PACKAGE) << "SceneManager: changed to scene \""
		<< scenes.at(currentScene)->getName() << "\"";

	setupScene(scenes.at(currentScene));
	sceneChangeTimer.setAlarm(SCENE_CHANGE_MS);
}

//--------------------------------------------------------------
void SceneManager::prevScene() {
	if(scenes.empty() || !sceneChangeTimer.alarm()) {
		return;
	}

	exit();
	currentScene--;
	if(currentScene < 0) {
		currentScene = scenes.size()-1;
	}

	ofLogVerbose(PACKAGE) << "SceneManager: changed to scene \""
		<< scenes.at(currentScene)->getName() << "\"";

	setupScene(scenes.at(currentScene));
	sceneChangeTimer.setAlarm(SCENE_CHANGE_MS);
}

//--------------------------------------------------------------
void SceneManager::gotoScene(unsigned int num) {
	if(scenes.empty() || !sceneChangeTimer.alarm()) {
		return;
	}

	if(num >= (int) scenes.size()) {
		ofLogWarning() << "SceneManager: cannot goto scene num " << num
			<< ", index out of range";
		return;
	}

	exit();
	currentScene = num;

	ofLogVerbose(PACKAGE) << "SceneManager: changed scene to \""
			  << scenes.at(currentScene)->getName() << "\"";

	setupScene(scenes.at(currentScene));
	sceneChangeTimer.setAlarm(SCENE_CHANGE_MS);
}

//--------------------------------------------------------------
void SceneManager::gotoScene(string name) {
	if(!sceneChangeTimer.alarm()) {
		return;
	}
	
	for(unsigned int i = 0; i < scenes.size(); ++i) {
		if(name == scenes.at(i)->getName()) {
			exit();
			currentScene = i;
			ofLogVerbose(PACKAGE) << "SceneManager: changed scene to \""
					  << scenes.at(currentScene)->getName() << "\"";
			setupScene(scenes.at(currentScene));
			sceneChangeTimer.setAlarm(SCENE_CHANGE_MS);
			return;
		}
	}
}

//--------------------------------------------------------------
void SceneManager::setup(bool loadAll) {
	
	// load scene change font
	if(!sceneNameFont.isLoaded()) {
		sceneNameFont.loadFont(Config::instance().fontFilename, SCENE_NAME_FONT_SIZE, false);
	}
	
	// setup all scenes
	if(loadAll) {
		for(unsigned int i = 0; i < scenes.size(); ++i) {
			scenes.at(i)->setup(true);
		}
	}
	else {
		if(currentScene > 0) {
			scenes.at(currentScene)->setup();
		}
	}

	// try to load the first scene
	if(currentScene < 0) {
		gotoScene(0);
	}
}

//--------------------------------------------------------------
void SceneManager::update() {
	if(currentScene > 0) {
		scenes[currentScene]->update();
	}
}

//--------------------------------------------------------------
void SceneManager::draw() {
	if(currentScene >= 0 && currentScene < (int) scenes.size()) {
		Scene* s = scenes.at(currentScene);
		
		if(bSetupScene) {
			s->setup();
			bSetupScene = false;
		}
		
		// only set background in gl state since scene changes may happen
		// in osc receiver thread
		if(bSetBackground) {
			ofBackground(s->getBackground());
			bSetBackground = false;
		}
		
		// artificial frame rate timing
		if(s->getFps() > 0) {
		if(frameRateTimer.alarm()) {
			s->draw();
			frameRateTimer.setAlarm(1000/s->getFps());
		}
		}
		else {
			s->draw();
		}
		
		if(bShowSceneName && !sceneNameTimer.alarm()) {
			ofSetHexColor(0xFF00FF);
			sceneNameFont.drawString(s->getName(),
				0, (int) Config::instance().renderHeight - sceneNameFont.stringHeight(s->getName())*0.25);
		}
	}
}

//--------------------------------------------------------------
void SceneManager::exit(bool exitAll) {
	if(exitAll) {
		for(unsigned int i = 0; i < scenes.size(); ++i) {
			scenes.at(i)->exit();
		}
	}
	else {
		if(currentScene > 0) {
			scenes.at(currentScene)->exit();
		}
	}
}

//--------------------------------------------------------------
void SceneManager::setFrameRate(unsigned int rate) {
	frameRate = rate;
	if(frameRate > 0) {
		frameRateTimer.setAlarm(0);
	}
}

// PROTECTED
//--------------------------------------------------------------
void SceneManager::setupScene(Scene* s) {
	bSetupScene = true;
	bSetBackground = true;
	setFrameRate(s->getFps());
	sceneNameTimer.setAlarm(SCENE_NAME_MS);
}

//--------------------------------------------------------------
bool SceneManager::processOscMessage(const ofxOscMessage& message) {

	if(currentScene >= 0 && currentScene < (int) scenes.size()) {

		Scene *scene = scenes.at(currentScene);

		if(message.getAddress() == getOscRootAddress() + "/scene/slideshow") {
			bool b;
			if(OscObject::tryBool(message, b, 0)) {
				scene->setSlideshow(b);
			}
			return true;
		}
		
		if(message.getAddress() == getOscRootAddress() + "/scene/object") {
		
			if(message.getArgType(0) == OFXOSC_TYPE_STRING) {
				string object = message.getArgAsString(0);
				scene->gotoObject(object);
				return true;
			}
			else if(message.getArgType(0) == OFXOSC_TYPE_INT32) {
				int index = message.getArgAsInt32(0);
				scene->gotoObject(index);
				return true;
			}
		}
		
		else if(message.getAddress() == getOscRootAddress() + "/scene/object/prev") {
			scene->prevObject();
			return true;
		}
		
		else if(message.getAddress() == getOscRootAddress() + "/scene/object/next") {
			scene->nextObject();
			return true;
		}

		return scene->processOsc(message);
	}

	return false;
}

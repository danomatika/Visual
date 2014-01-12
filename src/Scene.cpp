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
#include "Scene.h"

//--------------------------------------------------------------
Scene::Scene(string name, string parentOscAddress) :
	ofxScene(name, false), OscObject(parentOscAddress+"/"+name) {

}

//--------------------------------------------------------------
void Scene::setup()	{

}

//--------------------------------------------------------------
void Scene::update() {

}

//--------------------------------------------------------------
void Scene::draw() {

}

//--------------------------------------------------------------
void Scene::exit() {

}

//--------------------------------------------------------------		
//void Scene::keyPressed(int key) {global.scriptEngine.lua.scriptKeyPressed(key);}
//void Scene::keyReleased(int key) {global.scriptEngine.lua.scriptKeyReleased(key);}
//void Scene::mouseMoved(int x, int y) {global.scriptEngine.lua.scriptMouseMoved(x, y);}
//void Scene::mouseDragged(int x, int y, int button) {global.scriptEngine.lua.scriptMouseDragged(x, y, button);}
//void Scene::mousePressed(int x, int y, int button) {global.scriptEngine.lua.scriptMousePressed(x, y, button);}
//void Scene::mouseReleased(int x, int y, int button) {global.scriptEngine.lua.scriptMouseReleased(x, y, button);}

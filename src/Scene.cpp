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

#include "objects/Objects.h"

//--------------------------------------------------------------
Scene::Scene(string name) : _name(name), _background(0), _fps(-1) {
	// set address here as the baseAddress might have been changed in lua
	oscRootAddress = Config::instance().baseAddress+"/"+name;
}

//--------------------------------------------------------------
Scene::~Scene() {
	clearObjects();
}

//--------------------------------------------------------------
void Scene::addObject(DrawableObject* object) {
	if(object == NULL) {
		ofLogError() << "Scene \"" << _name << "\": cannot add NULL object";
		return;
	}

	object->setOscRootAddress(oscRootAddress+"/"+object->getName());
	addOscObject(object);
	_objectList.push_back(object);
	
	ofLogVerbose(PACKAGE) << "Scene \"" << _name << "\": added " << object->getType()
		<< " \"" << object->getName() << "\"";
}

//--------------------------------------------------------------
void Scene::removeObject(DrawableObject* object) {
	if(object == NULL) {
		ofLogError() << "Scene \"" << _name << "\": cannot remove NULL object";
		return;
	}

	vector<DrawableObject*>::iterator iter;
	iter = find(_objectList.begin(), _objectList.end(), object);
	if(iter != _objectList.end()) {
		removeOscObject((*iter));
		_objectList.erase(iter);
	}
}

//--------------------------------------------------------------
void Scene::clearObjects() {

	/// delete all the objects
	for(unsigned int i = 0; i < _objectList.size(); ++i) {
		DrawableObject* o = _objectList.at(i);
		removeOscObject(o);
		delete o;
	}
	_objectList.clear();
}

//--------------------------------------------------------------
void Scene::setup() {
	for(unsigned int i = 0; i < _objectList.size(); ++i) {
		_objectList.at(i)->setup();
	}
}

//--------------------------------------------------------------
void Scene::draw() {
	vector<DrawableObject*>::iterator iter;
	for(iter = _objectList.begin(); iter != _objectList.end(); ++iter) {
		
		// remove any NULL objects
		if((*iter) == NULL) {
			_objectList.erase(iter);
			ofLogError() << "Scene \"" << _name << "\": removed NULL object";
		}
		else {
			(*iter)->draw();
		}
	}
}

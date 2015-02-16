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
#include "Scene.h"

#include "objects/DrawableObject.h"

//--------------------------------------------------------------
Scene::Scene(string name) : bSetup(false), name(name), background(0), fps(-1),
	bSlideshow(false), currentObject(-1) {
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
		ofLogError() << "Scene \"" << name << "\": cannot add NULL object";
		return;
	}

	object->setOscRootAddress(oscRootAddress+"/"+object->getName());
	addOscObject(object);
	objects.push_back(object);
	
	ofLogVerbose(PACKAGE) << "Scene \"" << name << "\": added "
		<< object->getType() << " \"" << object->getName() << "\"";
}

//--------------------------------------------------------------
void Scene::removeObject(DrawableObject* object) {
	if(object == NULL) {
		ofLogError() << "Scene \"" << name << "\": cannot remove NULL object";
		return;
	}

	int i = 0;
	vector<DrawableObject*>::iterator iter;
	iter = find(objects.begin(), objects.end(), object);
	if(iter != objects.end()) {
		removeOscObject((*iter));
		if((iter - objects.begin()) == currentObject) {
			currentObject = -1; // reset if current object was removed
		}
		objects.erase(iter);
	}
}

//--------------------------------------------------------------
void Scene::clearObjects() {

	/// delete all the objects
	for(unsigned int i = 0; i < objects.size(); ++i) {
		DrawableObject* o = objects.at(i);
		removeOscObject(o);
		delete o;
	}
	objects.clear();
	currentObject = -1;
}

//--------------------------------------------------------------
void Scene::setup(bool earlySetup) {
	if(!bSetup) {
		for(unsigned int i = 0; i < objects.size(); ++i) {
			if(!earlySetup || objects[i]->allowsEarlySetup()) {
				objects[i]->setup();
			}
		}
		bSetup = true;
	}
	else {
		for(unsigned int i = 0; i < objects.size(); ++i) {
			if(objects[i]->shouldAlwaysBeSetup()) {
				objects[i]->setup();
			}
		}
	}
	
	// make sure to show first object in slide show
	if(bSlideshow) {
		gotoObject(0);
	}
}

//--------------------------------------------------------------
void Scene::update() {
	if(bSlideshow) {
		if(currentObject >= 0) {
			objects[currentObject]->update();
		}
	}
	else {
		for(unsigned int i = 0; i < objects.size(); ++i) {
			objects[i]->update();
		}
	}
}

//--------------------------------------------------------------
void Scene::draw() {
	if(bSlideshow) {
		if(currentObject >= 0) {
			objects[currentObject]->draw();
		}
	}
	else {
		vector<DrawableObject*>::iterator iter;
		for(iter = objects.begin(); iter != objects.end(); ++iter) {
			
			// remove any NULL objects
			if((*iter) == NULL) {
				objects.erase(iter);
				ofLogError() << "Scene \"" << name << "\": removed NULL object";
			}
			else {
				(*iter)->draw();
			}
		}
	}
}

//--------------------------------------------------------------
void Scene::exit() {
	for(unsigned int i = 0; i < objects.size(); ++i) {
		if(objects[i]->shouldClearOnExit()) {
			objects.at(i)->clear();
		}
	}
}

//--------------------------------------------------------------
void Scene::nextObject() {
	if(objects.empty()) {
		return;
	}

	currentObject++;
	if(currentObject >= (int) objects.size()) {
		currentObject = 0;
	}

	ofLogVerbose(PACKAGE) << "Scene \"" << name <<  "\": changed to object \""
		<< objects.at(currentObject)->getName() << "\"";
}

//--------------------------------------------------------------
void Scene::prevObject() {
	if(objects.empty()) {
		return;
	}

	currentObject--;
	if(currentObject < 0) {
		currentObject = objects.size()-1;
	}

	ofLogVerbose(PACKAGE) << "Scene \"" << name <<  "\": changed to object \""
		<< objects.at(currentObject)->getName() << "\"";
}

//--------------------------------------------------------------
void Scene::gotoObject(unsigned int num) {
	if(objects.empty()) {
		return;
	}

	if(num >= (int) objects.size()) {
		ofLogWarning() << "Scene \"" << name <<  "\": cannot goto object num " << num
			<< ", index out of range";
		return;
	}

	currentObject = num;

	ofLogVerbose(PACKAGE) << "Scene \"" << name <<  "\": changed object to \""
			  << objects.at(currentObject)->getName() << "\"";
}

//--------------------------------------------------------------
void Scene::gotoObject(string name) {
	for(unsigned int i = 0; i < objects.size(); ++i) {
		if(name == objects.at(i)->getName()) {
			currentObject = i;
			ofLogVerbose(PACKAGE) << "Scene \"" << this->name <<  "\": changed object to \""
					  << objects.at(currentObject)->getName() << "\"";
			return;
		}
	}
	
	ofLogWarning() << "Scene \"" << this->name <<  "\": cannot goto object \""
		<< name	<< "\", name not found";
}

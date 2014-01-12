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
	
	Adapted from https://github.com/danomatika/oscframework

==============================================================================*/
#include "OscObject.h"

//--------------------------------------------------------------
bool OscObject::processOsc(const ofxOscMessage& message) {

	// call any attached objects
	vector<OscObject*>::iterator objectIter;
	for(objectIter = _objectList.begin(); objectIter != _objectList.end();) {
		
		// try to process message
		if((*objectIter) != NULL) {
			if((*objectIter)->processOsc(message)) {
				return true;
			}
			objectIter++; // increment iter
		}
		else {    // bad object, so erase it
			objectIter = _objectList.erase(objectIter);
			ofLogWarning() << "OscObject: removed NULL object";
		}
	}

	return processOscMessage(message);
}

//--------------------------------------------------------------
void OscObject::addOscObject(OscObject* object) {
	if(object == NULL) {
		ofLogWarning() << "OscObject: cannot add NULL object";
		return;
	}
	_objectList.push_back(object);
}

//--------------------------------------------------------------
void OscObject::removeOscObject(OscObject* object) {
	if(object == NULL) {
		ofLogWarning() << "OscObject: cannot remove NULL object";
		return;
	}

	// find object in list and remove it
	vector<OscObject*>::iterator iter;
	iter = find(_objectList.begin(), _objectList.end(), object);
	if(iter != _objectList.end()) {
		_objectList.erase(iter);
	}
}

//--------------------------------------------------------------
void OscObject::setOscRootAddress(string rootAddress) {
	oscRootAddress = rootAddress;
}

//--------------------------------------------------------------
string& OscObject::getOscRootAddress() {
	return oscRootAddress;
}

//--------------------------------------------------------------
void OscObject::prependOscRootAddress(string prepend) {
	oscRootAddress = prepend + oscRootAddress;
}

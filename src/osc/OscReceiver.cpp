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
#include "OscReceiver.h"

//--------------------------------------------------------------
OscReceiver::OscReceiver(std::string rootAddress) :
	oscRootAddress(rootAddress),
	m_bIsRunning(false), m_bIgnoreMessages(false) {
	m_receiver = ofPtr<ofxOscReceiver>();
}

//--------------------------------------------------------------
OscReceiver::OscReceiver(unsigned int port, std::string rootAddress) :
	oscRootAddress(rootAddress),
	m_bIsRunning(false), m_bIgnoreMessages(false) {
	m_receiver = ofPtr<ofxOscReceiver>();
	setup(port);
}

//--------------------------------------------------------------
OscReceiver::~OscReceiver() {
	stop();
	_objectList.clear();
}

//--------------------------------------------------------------
bool OscReceiver::setup(unsigned int port) {
	if(m_receiver.get() != NULL) {
		ofLogWarning() << "OscReceiver: can't set port while thread is running";
		return false;
	}
	m_port = port;
	return true;
}

//--------------------------------------------------------------
void OscReceiver::update() {
	if(!m_bIsRunning) {
		return;
	}

	// check for waiting osc messages
	while(m_receiver->hasWaitingMessages()) {
		
		ofxOscMessage message;
		m_receiver->getNextMessage(&message);
	
		// ignore any incoming messages?
		if(m_bIgnoreMessages) continue;
		
		// call any attached objects
		vector<OscObject*>::iterator iter;
		for(iter = _objectList.begin(); iter != _objectList.end();) {
			
			// try to process message, if processed then done
			if((*iter) != NULL) {
				if((*iter)->processOsc(message)) {
					return true;
				}
				iter++; // increment iter
			}
			else {	// bad object, so erase it
				iter = _objectList.erase(iter);
				ofLogWarning() << "OscReceiver: removed NULL object";
			}
		}
	}
}

//--------------------------------------------------------------
void OscReceiver::start() {
	if(m_receiver.get() != NULL) {
		ofLogWarning() << "OscReceiver: can't start thread, already created";
		return;
	}

	m_receiver = ofPtr<ofxOscReceiver>(new ofxOscReceiver);
	if(m_receiver.get() == NULL) {
		ofLogWarning() << "OscReceiver: could not create thread";
		return false;
	}
	m_receiver->setup(m_port);
	
	m_bIsRunning = true;
}

//--------------------------------------------------------------
void OscReceiver::stop() {
	if(m_receiver.get() == NULL) {
		return;
	}
	m_receiver.reset();
	m_bIsRunning = false;
	m_bIgnoreMessages = false;
}

//--------------------------------------------------------------
void OscReceiver::addOscObject(OscObject *object) {
	if(object == NULL) {
		ofLogWarning() << "OscReceiver: can't add NULL object" << std::endl;
		return;
	}
	_objectList.push_back(object);
}

//--------------------------------------------------------------
void OscReceiver::removeOscObject(OscObject *object) {
	if(object == NULL) {
		ofLogWarning() << "OscReceiver: can't remove NULL object" << std::endl;
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
unsigned int OscReceiver::getPort() {
	return m_port;
}

//--------------------------------------------------------------
void OscReceiver::setOscRootAddress(std::string rootAddress) {
	oscRootAddress = rootAddress;
}

//--------------------------------------------------------------
string& OscReceiver::getOscRootAddress()	{
	return oscRootAddress;
}

//--------------------------------------------------------------
void OscReceiver::ignoreMessages(bool yesno) {
	m_bIgnoreMessages = yesno;
}

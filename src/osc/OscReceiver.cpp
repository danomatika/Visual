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
	
	Adapted from https://github.com/danomatika/oscframework

==============================================================================*/
#include "OscReceiver.h"

//--------------------------------------------------------------
OscReceiver::OscReceiver() :
	m_bIsRunning(false), m_bIgnoreMessages(false) {
	m_receiver = ofPtr<Receiver>();
}

//--------------------------------------------------------------
OscReceiver::OscReceiver(unsigned int port) :
	m_bIsRunning(false), m_bIgnoreMessages(false) {
	m_receiver = ofPtr<Receiver>();
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
void OscReceiver::start() {
	if(m_receiver.get() != NULL) {
		ofLogWarning() << "OscReceiver: can't start thread, already created";
		return;
	}

	m_receiver = ofPtr<Receiver>(new Receiver);
	if(m_receiver.get() == NULL) {
		ofLogWarning() << "OscReceiver: could not create thread";
		return false;
	}
	m_receiver->receiver = this;
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
	mutex.lock();
	_objectList.push_back(object);
	mutex.unlock();
}

//--------------------------------------------------------------
void OscReceiver::removeOscObject(OscObject *object) {
	if(object == NULL) {
		ofLogWarning() << "OscReceiver: can't remove NULL object" << std::endl;
		return;
	}

	// find object in list and remove it
	mutex.lock();
	vector<OscObject*>::iterator iter;
	iter = find(_objectList.begin(), _objectList.end(), object);
	if(iter != _objectList.end()) {
		_objectList.erase(iter);
	}
	mutex.unlock();
}

//--------------------------------------------------------------
unsigned int OscReceiver::getPort() {
	return m_port;
}

//--------------------------------------------------------------
void OscReceiver::ignoreMessages(bool yesno) {
	m_bIgnoreMessages = yesno;
}

//--------------------------------------------------------------
void OscReceiver::processMessage(const ofxOscMessage &message) {
	
	// ignore any incoming messages?
	if(m_bIgnoreMessages) return;
	
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

//--------------------------------------------------------------
void OscReceiver::Receiver::ProcessMessage(const osc::ReceivedMessage &m,
										   const osc::IpEndpointName& remoteEndpoint) {
	
	// convert the message to an ofxOscMessage
	ofxOscMessage message;

	// set the address
	message.setAddress(m.AddressPattern());

	// set the sender ip/host
	char endpoint_host[osc::IpEndpointName::ADDRESS_STRING_LENGTH];
	remoteEndpoint.AddressAsString(endpoint_host);
    message.setRemoteEndpoint(endpoint_host, remoteEndpoint.port);

	// transfer the arguments
	for(osc::ReceivedMessage::const_iterator arg = m.ArgumentsBegin();
		arg != m.ArgumentsEnd(); ++arg) {
		if(arg->IsInt32()) {
			message.addIntArg(arg->AsInt32Unchecked());
		}
		else if(arg->IsInt64()) {
			message.addInt64Arg(arg->AsInt64Unchecked());
		}
		else if(arg->IsFloat()) {
			message.addFloatArg(arg->AsFloatUnchecked());
		}
		else if(arg->IsString()) {
			message.addStringArg(arg->AsStringUnchecked());
		}
		else {
			ofLogError() << "OscReceiver: argument in message "
				<< m.AddressPattern() << " is not an int, float, or string";
		}
	}
	
	// send
	receiver->mutex.lock();
	receiver->processMessage(message);
	receiver->mutex.unlock();
}

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
#include "App.h"

//#include "Scene.h"

//--------------------------------------------------------------
App::App() : OscObject(""), config(Config::instance()),
	receiver(Config::instance().oscReceiver),
	sender(Config::instance().oscSender) {
	
	shiftPressed = false;
	bDebug = false;
	bRunning = true;
	reloadTimestamp = 0;
	saveTimestamp = 0;
	
	// set osc addresses
	setOscRootAddress(Config::instance().baseAddress);
	receiver.setOscRootAddress(Config::instance().baseAddress);
	
//	ofLogNotice() << "SCENE MANAGER " << sceneManager.getOscRootAddress() << endl;

	// add osc objects
	receiver.addOscObject(this);
//	receiver.addOscObject(&sceneManager);
}

//--------------------------------------------------------------
void App::setup() {
	
	ofSetLogLevel("visual", OF_LOG_VERBOSE);
	
	ofBackground(0);
	
	// don't show the cursor if not debugging
	if(bDebug) {
		ofShowCursor();
	}
	else {
		ofHideCursor();
	}

	// setup the osc receiver
	receiver.setup(9990);
	receiver.start();
	
	// setup the osc sender
	sender.setup(config.sendingIp, config.sendingPort);
	
	config.print();
	config.setup();
	
	ofSetVerticalSync(true);
	
	// try to load first scene
//	sceneManager.setup();
	
	// notify of connection
	ofxOscMessage message;
	message.setAddress(config.notificationAddress);
	message.addStringArg("connect");
	message.addIntArg(config.connectionId);
	sender.sendMessage(message);
}

//--------------------------------------------------------------
void App::update() {
	receiver.update();
	sceneManager.update();
}

//--------------------------------------------------------------
void App::draw() {
	sceneManager.draw();
}

//--------------------------------------------------------------
void App::exit() {

	// notify of disconnection
	ofxOscMessage message;
	message.setAddress(config.notificationAddress);
	message.addStringArg("disconnect");
	message.addIntArg(config.connectionId);
	sender.sendMessage(message);

	receiver.stop();
	sceneManager.clear();
}

//--------------------------------------------------------------
void App::keyPressed(int key) {
	
	switch(key) {
	
		case OF_KEY_SHIFT:
			shiftPressed = true;
			break;
	
		case OF_KEY_UP: {
			ofxOscMessage message;
			message.setAddress(config.deviceAddress + "/keyboard");
			message.addStringArg("up");
			sender.sendMessage(message);
			return;
		}
			
		case OF_KEY_DOWN: {
			ofxOscMessage message;
			message.setAddress(config.deviceAddress + "/keyboard");
			message.addStringArg("up");
			sender.sendMessage(message);
			return;
		}
	
		case OF_KEY_LEFT: {
			if(shiftPressed) {
//				sceneManager.prevScene();
//				return;
			}
			ofxOscMessage message;
			message.setAddress(config.deviceAddress + "/keyboard");
			message.addStringArg("left");
			sender.sendMessage(message);
			return;
		}

		case OF_KEY_RIGHT: {
			if(shiftPressed) {
//				sceneManager.prevScene();
//				return;
			}
			ofxOscMessage message;
			message.setAddress(config.deviceAddress + "/keyboard");
			message.addStringArg("right");
			sender.sendMessage(message);
			return;
		}
		
		case 'P':
			bRunning = !bRunning;
			receiver.ignoreMessages(!bRunning);
			ofLogVerbose("visual") << "Running: " << bRunning;
			return;

		case 'D':
			bDebug = !bDebug;
			if(bDebug) {
				ofShowCursor();
			}
			else {
				ofHideCursor();
			}
			ofLogVerbose("visual") << "Debug: " << bDebug;
			return;

		case 'R':
			if(ofGetElapsedTimeMillis() - reloadTimestamp > 5000) {
//				ofLogNotice() << "Reloading xml file: " << ofFilePath::getFileName(sceneManager.getXmlFilename());
//				sceneManager.reload();
				reloadTimestamp = ofGetElapsedTimeMillis();
				return;
			}
			break;
			
		case 'S':
			if(ofGetElapsedTimeMillis() - saveTimestamp > 5000) {
//				ofLogNotice() << "Saving xml file: " << ofFilePath::getFileName(sceneManager.getXmlFilename());
//				sceneManager.saveXmlFile();
				saveTimestamp = ofGetElapsedTimeMillis();
				return;
			}
			break;
		
		case 'F':
			ofToggleFullscreen();
			break;
	}
	
	sceneManager.keyPressed(key);
}

//--------------------------------------------------------------
void App::keyReleased(int key) {
	
	switch(key) {
	
		case OF_KEY_SHIFT:
			shiftPressed = false;
			break;
	}
	
	sceneManager.keyReleased(key);
}

//--------------------------------------------------------------
void App::mouseMoved(int x, int y ) {
	sceneManager.mouseMoved(x, y);
}

//--------------------------------------------------------------
void App::mouseDragged(int x, int y, int button) {
	sceneManager.mousePressed(x, y, button);
}

//--------------------------------------------------------------
void App::mousePressed(int x, int y, int button) {
	sceneManager.mouseReleased(x, y, button);
}

//--------------------------------------------------------------
void App::mouseReleased(int x, int y, int button) {
	sceneManager.mouseDragged(x, y, button);
}

//--------------------------------------------------------------
void App::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void App::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void App::dragEvent(ofDragInfo dragInfo) { 

}

// PROTECTED
//--------------------------------------------------------------
bool App::processOscMessage(const ofxOscMessage& message) {

	ofLogVerbose("visual") << "received " << message.getAddress();
	
	if(message.getAddress() == getOscRootAddress() + "/scene") {
	
		if(message.getArgType(0) == OFXOSC_TYPE_STRING) {
			string scene = message.getArgAsString(0);
			sceneManager.gotoScene(scene);
			return true;
		}
		else if(message.getArgType(0) == OFXOSC_TYPE_INT32) {
			int index = message.getArgAsInt32(0);
			sceneManager.gotoScene(index);
			return true;
		}
	}
	
	else if(message.getAddress() == getOscRootAddress() + "/scene/prev") {
		sceneManager.prevScene();
		return true;
	}
	
	else if(message.getAddress() == getOscRootAddress() + "/scene/next") {
		sceneManager.nextScene();
		return true;
	}

	else if(message.getAddress() == getOscRootAddress() + "/file"
		&& message.getArgType(0) == OFXOSC_TYPE_STRING) {
	
//		if(bSceneFileIsConfigFile) {
//		
//			// don't save config data to new file
//			sceneManager.removeXmlObject(&config);
//			bSceneFileIsConfigFile = false;
//		}
//		sceneManager.loadXmlFile(message.getArgAsString(0));
		return true;
	}
	
	else if(message.getAddress() == getOscRootAddress() + "/reload") {
//		sceneManager.reload();
		return true;
	}
	
	else if(message.getAddress() == getOscRootAddress() + "/framerate") {
	
		if(message.getArgType(0) == OFXOSC_TYPE_INT32) {
			ofSetFrameRate(message.getArgAsInt32(0));
		}
		else if (message.getArgType(0) == OFXOSC_TYPE_FLOAT) {
			ofSetFrameRate(message.getArgAsFloat(0));
		}
		return true;
	}

	else if(message.getAddress() == getOscRootAddress() + "/quit") {
		exitApp();
		return true;
	}

	return false;
}

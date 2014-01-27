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

#include "Scene.h"

//--------------------------------------------------------------
App::App() : config(Config::instance()),
	receiver(Config::instance().oscReceiver),
	sender(Config::instance().oscSender),
	scriptEngine(Config::instance().scriptEngine) {
	
	shiftPressed = false;
	bDebug = false;
	bRunning = true;
	reloadTimestamp = 0;
	saveTimestamp = 0;
	bConfigScript = false;
	
	// set osc addresses
	setOscRootAddress(config.baseAddress);
	sceneManager.setOscRootAddress(config.baseAddress);

	// add osc objects
	receiver.addOscObject(this);
	
	// translate origin, scale to screen size,
	// no quad warp, keep aspect ratio, center when scaling
	transformer.setTransforms(true, true, false, true, true);
}

//--------------------------------------------------------------
void App::setup() {
	
	ofSetLogLevel(PACKAGE, OF_LOG_VERBOSE);
	ofSetVerticalSync(true);
	ofBackground(0);
	//ofHideCursor();
	
	// read from the app bundle
	#ifdef __APPLE__
		ofSetDataPathRoot("../Resources/data/");
		ofLogVerbose(PACKAGE) << "Data path is \"" << ofToDataPath("")  << "\"";
	#endif
	
	// set render size
	if(config.renderWidth == 0 && config.renderHeight == 0) {
		config.renderWidth = ofGetWidth();
		config.renderHeight = ofGetHeight();
	}
	transformer.setRenderSize(config.renderWidth, config.renderHeight);
	
	// load fonts
	config.setup();
	sceneManager.setup();
	
	// load lua config / playlist
	loadConfigScript();
	config.print();
	
	// don't show the cursor if not debugging
	if(bDebug) {
		ofShowCursor();
	}
	
	// setup the osc receiver
	receiver.setup(config.listeningPort);
	receiver.start();
	
	// setup the osc sender
	sender.setup(config.sendingIp, config.sendingPort);
	
	// setup & try to load first scene
	sceneManager.setup(config.setupAllScenes);
	
	// notify of connection
	ofxOscMessage message;
	message.setAddress(config.notificationAddress);
	message.addStringArg("connect");
	message.addIntArg(config.connectionId);
	sender.sendMessage(message);
}

//--------------------------------------------------------------
void App::update() {
	mutex.lock();
	
	sceneManager.update();
//	config.resourceManager.update();
	scriptEngine.lua.scriptUpdate();

	mutex.unlock();
}

//--------------------------------------------------------------
void App::draw() {
	mutex.lock();
	
	transformer.pushTransforms();

		sceneManager.draw();
		scriptEngine.lua.scriptDraw();
	
	transformer.popTransforms();
	
	if(bDebug) {
		ofSetColor(255);
		ofDrawBitmapStringHighlight(ofToString((int) ofGetFrameRate()), 0, 12);
	}
	
	mutex.unlock();
}

//--------------------------------------------------------------
void App::exit() {

	// notify of disconnection
	ofxOscMessage message;
	message.setAddress(config.notificationAddress);
	message.addStringArg("disconnect");
	message.addIntArg(config.connectionId);
	sender.sendMessage(message);

	scriptEngine.lua.scriptExit();
	receiver.stop();
	sceneManager.clear();
	
	ofLog() << "exiting ...";
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
			message.addStringArg("down");
			sender.sendMessage(message);
			return;
		}
	
		case OF_KEY_LEFT: {
			if(shiftPressed) {
				sceneManager.prevScene();
				return;
			}
			ofxOscMessage message;
			message.setAddress(config.deviceAddress + "/keyboard");
			message.addStringArg("left");
			sender.sendMessage(message);
			return;
		}

		case OF_KEY_RIGHT: {
			if(shiftPressed) {
				sceneManager.nextScene();
				return;
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
			ofLogVerbose(PACKAGE) << "Running: " << bRunning;
			return;

		case 'D':
			bDebug = !bDebug;
			if(bDebug) {
				ofShowCursor();
			}
			else {
				ofHideCursor();
			}
			ofLogVerbose(PACKAGE) << "Debug: " << bDebug;
			return;

		case 'R':
			if(ofGetElapsedTimeMillis() - reloadTimestamp > 5000) {
				if(bConfigScript) {
					reloadConfigScript();
				}
				else {
					reloadScript();
				}
				reloadTimestamp = ofGetElapsedTimeMillis();
				return;
			}
			break;
		
		case 'F':
			ofToggleFullscreen();
			break;
	}
	
	// forward key events
	ofxOscMessage message;
	message.setAddress(config.deviceAddress + "/keyboard");
	message.addIntArg(key);
	sender.sendMessage(message);
	
	scriptEngine.lua.scriptKeyPressed(key);
}

//--------------------------------------------------------------
void App::keyReleased(int key) {
	
	switch(key) {
	
		case OF_KEY_SHIFT:
			shiftPressed = false;
			break;
	}
	
	scriptEngine.lua.scriptKeyReleased(key);
}

//--------------------------------------------------------------
void App::mouseMoved(int x, int y ) {
	scriptEngine.lua.scriptMouseMoved(x, y);
}

//--------------------------------------------------------------
void App::mouseDragged(int x, int y, int button) {
	scriptEngine.lua.scriptMousePressed(x, y, button);
}

//--------------------------------------------------------------
void App::mousePressed(int x, int y, int button) {
	scriptEngine.lua.scriptMouseReleased(x, y, button);
}

//--------------------------------------------------------------
void App::mouseReleased(int x, int y, int button) {
	scriptEngine.lua.scriptMouseDragged(x, y, button);
}

//--------------------------------------------------------------
void App::windowResized(int w, int h) {
	// set up transforms with new screen size
	transformer.setNewScreenSize(w, h);
}

//--------------------------------------------------------------
void App::gotMessage(ofMessage msg) {}

//--------------------------------------------------------------
void App::dragEvent(ofDragInfo dragInfo) {}

//--------------------------------------------------------------
void App::loadConfigScript() {
	if(!config.file.empty()) {
		ofLogNotice() << "loading \"" << config.file << "\"";
		
		// set data path to config file folder
		ofSetDataPathRoot(ofFilePath::getEnclosingDirectory(config.file));
		
		scriptEngine.loadScript(config.file);
		bConfigScript = true;
	}
}

//--------------------------------------------------------------
void App::reloadConfigScript() {
	if(!config.file.empty()) {
		ofLogNotice() << "reloading  \"" << config.file << "\"";
		
		sceneManager.clear(true);
		config.resourceManager.clear();
		scriptEngine.reloadScript();
		sceneManager.setup(config.setupAllScenes);
	}
}

//--------------------------------------------------------------
void App::reloadScript() {
	if(scriptEngine.getCurrentScript() != "") {
		scriptEngine.lua.scriptExit();
		scriptEngine.reloadScript();
	}
}

// PROTECTED
//--------------------------------------------------------------
// lock scene changes with the mutex to make sure the lua state is
// closed before calling lua functions
bool App::processOscMessage(const ofxOscMessage& message) {

//	ofLogVerbose(PACKAGE) << "received " << message.getAddress();
	
	if(message.getAddress() == getOscRootAddress() + "/scene") {
	
		if(message.getArgType(0) == OFXOSC_TYPE_STRING) {
			string scene = message.getArgAsString(0);
			mutex.lock();
				sceneManager.gotoScene(scene);
			mutex.unlock();
			return true;
		}
		else if(message.getArgType(0) == OFXOSC_TYPE_INT32) {
			int index = message.getArgAsInt32(0);
			mutex.lock();
				sceneManager.gotoScene(index);
			mutex.unlock();
			return true;
		}
	}
	
	else if(message.getAddress() == getOscRootAddress() + "/scene/prev") {
		mutex.lock();
			sceneManager.prevScene();
		mutex.unlock();
		return true;
	}
	
	else if(message.getAddress() == getOscRootAddress() + "/scene/next") {
		mutex.lock();
			sceneManager.nextScene();
		mutex.unlock();
		return true;
	}

	else if(message.getAddress() == getOscRootAddress() + "/file"
		&& message.getArgType(0) == OFXOSC_TYPE_STRING) {
		if(tryString(message, config.file, 0)) {
			loadConfigScript();
		}
		return true;
	}
	
	else if(message.getAddress() == getOscRootAddress() + "/reload") {
		reloadConfigScript();
		return true;
	}
	
	else if(message.getAddress() == getOscRootAddress() + "/framerate") {
		unsigned int fps;
		if(OscObject::tryNumber(message, fps, 0)) {
			sceneManager.setFrameRate(fps);
		}
		return true;
	}

	else if(message.getAddress() == getOscRootAddress() + "/quit") {
		exitApp();
		return true;
	}
	
	if(sceneManager.processOsc(message)) {
		return true;
	}
	
	// forward message to lua
	scriptEngine.sendOsc(message);

	return true;
}

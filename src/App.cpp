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

#define RELOAD_TIMEOUT_MS 1000

//--------------------------------------------------------------
App::App() : config(Config::instance()),
	receiver(Config::instance().oscReceiver),
	sender(Config::instance().oscSender),
	scriptEngine(Config::instance().scriptEngine) {
	
	bDebug = false;
	bRunning = true;
	reloadTimestamp = 0;
	saveTimestamp = 0;
	bUpdateCursor = false;
	bUpdateWindowShape = false;
	bGoFullscreen = false;
	
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
	
	#ifdef DEBUG
		ofSetLogLevel(PACKAGE, OF_LOG_VERBOSE);
	#endif
	ofSetVerticalSync(true);
	//ofSetFrameRate(30);
	ofBackground(0);
	
	// read from the app bundle
	#ifdef TARGET_OSX
		ofSetDataPathRoot("../Resources/data/");
		ofLogVerbose(PACKAGE) << "Data path is \"" << ofToDataPath("")  << "\"";
	#endif
	
	// load fonts, set default render size
	config.setup();
	sceneManager.setup();
	
	// setup editor & add editor event listening
	ofxEditor::loadFont(CONFIG_FONT, 24);
	editor.setup(this, true);
	
	// load lua script (if one was given)
	if(!config.script.empty()) {
		loadScript(config.script);
	}
	else {
		scriptEngine.setup();
		ofSetDataPathRoot(ofFilePath::addTrailingSlash(ofFilePath::getUserHomeDir()));
		editor.setCurrentEditor(0); // start with Repl
	}
	editor.setPath(ofToDataPath(""));
	config.print();
	
	// new render size?
	if(config.renderWidth != ofGetWidth() || config.renderHeight != ofGetHeight()) {
		bUpdateWindowShape = true;
	}
	transformer.setRenderSize(config.renderWidth, config.renderHeight);
	
	// go fullscreen & hide cursor
	if(config.fullscreen) {
		bGoFullscreen = true;
		bUpdateCursor = true;
	}
	
	// setup the osc receiver
	receiver.setup(config.listeningPort);
	receiver.start();
	
	// setup the osc sender
	sender.setup(config.sendingIp, config.sendingPort);
	
	// setup & try to load first scene
	sceneManager.showSceneName(config.showSceneNames);
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

	// update the window shape if we're using a different rendering size,
	// this dosen't seem to work in setup() so do it here
	if(bUpdateWindowShape) {// && ofGetFrameNum() > 1) {
		ofSetWindowShape(config.renderWidth, config.renderHeight);
		bUpdateWindowShape = false;
	}
	
	// again, do this here or it won't work with reshaping in setup()
	if(bGoFullscreen) {
		ofSetFullscreen(true);
		bGoFullscreen = false;
	}

	// show/hide the cursor here once the window has been setup
	if(bUpdateCursor && ofGetFrameNum() > 1) {
		if(ofGetWindowMode() != OF_WINDOW && !bDebug) {
			ofHideCursor();
		}
		else {
			ofShowCursor();
		}
		bUpdateCursor = false;
	}

	if(bRunning) {
		mutex.lock();
			sceneManager.update();
//			config.resourceManager.update();
			scriptEngine.lua.scriptUpdate();
		mutex.unlock();
	}
}

//--------------------------------------------------------------
void App::draw() {
	
	transformer.pushTransforms();
		mutex.lock();
			sceneManager.draw();
			scriptEngine.lua.scriptDraw();
		mutex.unlock();
	transformer.popTransforms();
	
	editor.draw();
	
	if(bDebug) {
		ofSetColor(255);
		ofDrawBitmapStringHighlight(ofToString((int) ofGetFrameRate()), 0, 12);
		
		if(!bRunning) {
			ofDrawBitmapStringHighlight("Paused", 0, ofGetHeight()-22);
		}
		
		Scene *s = sceneManager.getCurrentScene();
		if(s) {
			ofDrawBitmapStringHighlight(s->getName(), 0, ofGetHeight()-8);
		}
		else if(!config.script.empty()) {
			ofDrawBitmapStringHighlight(ofFilePath::getFileName(config.script), 0, ofGetHeight()-8);
		}
	}
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
	
	bool shiftPressed = ofGetKeyPressed(OF_KEY_SHIFT);
	#ifdef TARGET_OSX
		bool modifierPressed = ofGetKeyPressed(OF_KEY_COMMAND);
	#else
		bool modifierPressed = ofGetKeyPressed(OF_KEY_CONTROL);
	#endif

	switch(key) {
	
		case OF_KEY_UP: {
			if(editor.isHidden()) {
				ofxOscMessage message;
				message.setAddress(config.deviceAddress + "/keyboard");
				message.addStringArg("up");
				sender.sendMessage(message);
				return;
			}
			break;
		}
			
		case OF_KEY_DOWN: {
			if(editor.isHidden()) {
				ofxOscMessage message;
				message.setAddress(config.deviceAddress + "/keyboard");
				message.addStringArg("down");
				sender.sendMessage(message);
				return;
			}
			break;
		}
	
		case OF_KEY_LEFT: {
			if(editor.isHidden()) {
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
			break;
		}

		case OF_KEY_RIGHT: {
			if(editor.isHidden()) {
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
			break;
		}
		
		case 'p':
			if(editor.isHidden() && modifierPressed) {
				bRunning = !bRunning;
				receiver.ignoreMessages(!bRunning);
				ofLogVerbose(PACKAGE) << "Running: " << bRunning;
				return;
			}
			break;

		case 'd':
			if(editor.isHidden() && modifierPressed) {
				bDebug = !bDebug;
				bUpdateCursor = true;
				ofLogVerbose(PACKAGE) << "Debug: " << bDebug;
				return;
			}
			break;

		case 'E':
			// reload script & execute
			if(modifierPressed && shiftPressed) {
				if(ofGetElapsedTimeMillis() - reloadTimestamp > RELOAD_TIMEOUT_MS) {
					reloadScript();
					reloadTimestamp = ofGetElapsedTimeMillis();
					return;
				}
			}
			break;
		
		case 'f': case 6:
			if(modifierPressed) {
				ofToggleFullscreen();
				bUpdateCursor = true;
				return;
			}
			break;
			
		case 'l': case 12:
			if(modifierPressed) {
				editor.setLineWrapping(!editor.getLineWrapping());
				return;
			}
			break;
		
		case 'n': case 14:
			if(modifierPressed) {
				editor.setLineNumbers(!editor.getLineNumbers());
				return;
			}
			break;
		
		case 'z': case 26:
			if(modifierPressed) {
				editor.setAutoFocus(!editor.getAutoFocus());
				return;
			}
			break;
	}
	
	if(editor.isHidden()) {
		if(key < 256) { // forward ASCII key events
			ofxOscMessage message;
			message.setAddress(config.deviceAddress + "/keyboard");
			message.addIntArg(key);
			sender.sendMessage(message);
		}
		scriptEngine.lua.scriptKeyPressed(key);
	}
	editor.keyPressed(key);
}

//--------------------------------------------------------------
void App::keyReleased(int key) {
	scriptEngine.lua.scriptKeyReleased(key);
}

//--------------------------------------------------------------
void App::mouseMoved(int x, int y ) {
	scriptEngine.lua.scriptMouseMoved(x, y);
}

//--------------------------------------------------------------
void App::mouseDragged(int x, int y, int button) {
	scriptEngine.lua.scriptMouseDragged(x, y, button);
}

//--------------------------------------------------------------
void App::mousePressed(int x, int y, int button) {
	scriptEngine.lua.scriptMousePressed(x, y, button);
}

//--------------------------------------------------------------
void App::mouseReleased(int x, int y, int button) {
	scriptEngine.lua.scriptMouseReleased(x, y, button);
}

//--------------------------------------------------------------
void App::windowResized(int w, int h) {
	
	// set up transforms with new screen size
	transformer.setNewScreenSize(w, h);

	editor.resize(w, h);
}

//--------------------------------------------------------------
void App::dragEvent(ofDragInfo dragInfo) {
	if(dragInfo.files.empty()) return;
	
	string script = dragInfo.files[0];
	if(ofFilePath::getFileExt(script) != "lua") {
		ofLogError(PACKAGE) << "given script, << \"" << script << " is not a lua file";
		return;
	}
	
	unloadScript();
	loadScript(script);
	
	// new render size?
	if(config.renderWidth != ofGetWidth() || config.renderHeight != ofGetHeight()) {
		bUpdateWindowShape = true;
	}
	transformer.setRenderSize(config.renderWidth, config.renderHeight);
	
	// go fullscreen & hide cursor
	if(config.fullscreen) {
		bGoFullscreen = true;
		bUpdateCursor = true;
	}
}

//--------------------------------------------------------------
void App::openFileEvent(int &whichEditor){
	ofLogNotice() << "editor " << whichEditor << ": opened "
		<< ofFilePath::getFileName(editor.getEditorFilename(whichEditor));
}

//--------------------------------------------------------------
void App::saveFileEvent(int &whichEditor){
	ofLogNotice() << "editor " << whichEditor << ": saved "
		<< ofFilePath::getFileName(editor.getEditorFilename(whichEditor));
}

//--------------------------------------------------------------
void App::executeScriptEvent(int &whichEditor){
	scriptEngine.evalString(editor.getText(whichEditor), !editor.isSelection());
}

//--------------------------------------------------------------
void App::evalReplEvent(const string &text) {
	scriptEngine.evalString(text);
	editor.evalReplReturn();
}

//--------------------------------------------------------------
bool App::loadScript(string script) {
	if(script == "") return;

	ofLogNotice() << "loading \"" << script << "\"";
	
	// set data path to config file folder
	ofSetDataPathRoot(ofFilePath::getEnclosingDirectory(script));
	
	config.script = script;
	config.isPlaylist = false;
	if(!scriptEngine.loadScript(script)) {
		return false;
	}
	editor.openFile(script);
	
	// load settings
	if(config.isPlaylist) {
		sceneManager.showSceneName(config.showSceneNames);
		sceneManager.gotoScene(0);
		config.playlist = script;
	}
	
	return true;
}

//--------------------------------------------------------------
void App::reloadScript() {
	if(config.script == "") return;
	
	ofLogNotice() << "reloading \"" << config.script << "\"";
	
	if(config.isPlaylist) {
		sceneManager.clear(true);
		config.resourceManager.clear();
		scriptEngine.reloadScript();
		sceneManager.setup(config.setupAllScenes);
		config.playlist = config.script;
	}
	else {
		scriptEngine.reloadScript(); // calls scriptExit
	}
}

//--------------------------------------------------------------
void App::unloadScript() {
	if(config.script == "") return;
	
	ofLogNotice() << "unloading \"" << config.script << "\"";
	
	if(config.isPlaylist) {
		scriptEngine.unloadScript();
		sceneManager.clear();
		config.resourceManager.clear();
	}
	else {
		scriptEngine.unloadScript(); // calls scriptExit
	}
	config.script = "";
	
	if(!config.playlist.empty()) {
		config.script = config.playlist;
		scriptEngine.setCurrentScript(config.playlist);
	}
}

// PROTECTED
//--------------------------------------------------------------
// lock scene changes with the mutex to make sure the lua state is
// closed before calling lua functions
bool App::processOscMessage(const ofxOscMessage& message) {

	ofLogVerbose(PACKAGE) << "received " << message.getAddress();
	
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
		if(message.getArgType(0) == OFXOSC_TYPE_FLOAT && message.getArgAsFloat(0) == 0) {
			return true;
		}
		mutex.lock();
			sceneManager.prevScene();
		mutex.unlock();
		return true;
	}
	
	else if(message.getAddress() == getOscRootAddress() + "/scene/next") {
		if(message.getArgType(0) == OFXOSC_TYPE_FLOAT && message.getArgAsFloat(0) == 0) {
			return true;
		}
		mutex.lock();
			
			sceneManager.nextScene();
		mutex.unlock();
		return true;
	}

	else if(message.getAddress() == getOscRootAddress() + "/file"
		&& message.getArgType(0) == OFXOSC_TYPE_STRING) {
		if(tryString(message, config.script, 0)) {
			loadScript(config.script);
		}
		return true;
	}
	
	else if(message.getAddress() == getOscRootAddress() + "/reload") {
		reloadScript();
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
		ofExit();
		return true;
	}
	
	if(sceneManager.processOsc(message)) {
		return true;
	}
	
	// forward message to lua
	scriptEngine.sendOsc(message);

	return true;
}

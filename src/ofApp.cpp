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

==============================================================================*/
#include "ofApp.h"

#include "Scene.h"

#define RELOAD_TIMEOUT_MS 1000

//--------------------------------------------------------------
ofApp::ofApp() : config(Config::instance()),
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
void ofApp::setup() {
	
	#ifdef DEBUG
		ofSetLogLevel(PACKAGE, OF_LOG_VERBOSE);
	#endif
	ofSetVerticalSync(true);
	ofSetFrameRate(60); // set this as well for when window is hidden
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
	ofxRepl::setReplBanner("Welcome to Visual\nType help() for info");
	editor.setup(this, true);
	editor.setFlashEvalSelection(true);
	editor.setCurrentEditor(0); // start with Repl
	ofxEditor::setAutoFocusMinScale(0.75);
	
	// lua syntax
	syntax.loadFile("luaSyntax.xml");
	editor.getSettings().addSyntax(&syntax);
    
    // syntax highlighter colors
    colorScheme.loadFile("colorScheme.xml");
    editor.setColorScheme(&colorScheme);
	
	// load lua script (if one was given)
	if(!config.script.empty()) {
		loadScript(config.script);
	}
	else {
		scriptEngine.setup();
		ofSetDataPathRoot(ofFilePath::addTrailingSlash(ofFilePath::getUserHomeDir()));
	}
	editor.setPath(ofToDataPath("", true));
	if(bDebug) {
		config.print();
	}
	
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
void ofApp::update() {

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
		sceneManager.update();
//			config.resourceManager.update();
		scriptEngine.lua.scriptUpdate();
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	
	transformer.push();
		sceneManager.draw();
		scriptEngine.lua.scriptDraw();
	transformer.pop();
	
	editor.draw();
	
	if(bDebug) {
		ofSetColor(255);
		ofDrawBitmapStringHighlight(ofToString((int) ofGetFrameRate()), 0, 12);
		
		if(!bRunning) {
			ofDrawBitmapStringHighlight("Paused", 0, ofGetHeight()-22);
		}
		
		Scene *s = sceneManager.getCurrentScene();
		if(s) {
			ofDrawBitmapStringHighlight(s->getName(), 0, ofGetHeight()-6);
		}
		else if(!config.script.empty()) {
			ofDrawBitmapStringHighlight(ofFilePath::getFileName(config.script), 0, ofGetHeight()-6);
		}
	}
}

//--------------------------------------------------------------
void ofApp::exit() {

	// notify of disconnection
	ofxOscMessage message;
	message.setAddress(config.notificationAddress);
	message.addStringArg("disconnect");
	message.addIntArg(config.connectionId);
	sender.sendMessage(message);

	scriptEngine.lua.scriptExit();
	receiver.stop();
	sceneManager.clear();
	
	ofLogVerbose(PACKAGE) << "exiting ...";
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	
	bool shiftPressed = ofGetKeyPressed(OF_KEY_SHIFT);
	#ifdef TARGET_OSX
		bool modifierPressed = ofGetKeyPressed(OF_KEY_COMMAND);
	#else
		bool modifierPressed = ofGetKeyPressed(OF_KEY_CONTROL);
	#endif

	switch(key) {
	
		case OF_KEY_UP: {
			if(modifierPressed) {
				if(shiftPressed && sceneManager.getCurrentScene()->getSlideshow()) {
					sceneManager.getCurrentScene()->nextObject();
					return;
				}
			}
			else if(editor.isHidden()) {
				ofxOscMessage message;
				message.setAddress(config.deviceAddress + "/keyboard");
				message.addStringArg("up");
				sender.sendMessage(message);
				return;
			}
			break;
		}
			
		case OF_KEY_DOWN: {
			if(modifierPressed) {
				if(shiftPressed && sceneManager.getCurrentScene()->getSlideshow()) {
					sceneManager.getCurrentScene()->prevObject();
					return;
				}
			}
			else if(editor.isHidden()) {
				ofxOscMessage message;
				message.setAddress(config.deviceAddress + "/keyboard");
				message.addStringArg("down");
				sender.sendMessage(message);
				return;
			}
			break;
		}
	
		case OF_KEY_LEFT: {
			if(modifierPressed) {
				if(shiftPressed) {
					sceneManager.prevScene();
					return;
				}
			}
			else if(editor.isHidden()) {
				ofxOscMessage message;
				message.setAddress(config.deviceAddress + "/keyboard");
				message.addStringArg("left");
				sender.sendMessage(message);
				return;
			}
			break;
		}

		case OF_KEY_RIGHT: {		
			if(modifierPressed) {
				if(shiftPressed) {
					sceneManager.nextScene();
					return;
				}
			}
			else if(editor.isHidden()) {
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

		case 'e':
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
		
		case 'k': case 26:
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
void ofApp::keyReleased(int key) {
	scriptEngine.lua.scriptKeyReleased(key);
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ) {
	scriptEngine.lua.scriptMouseMoved(x, y);
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
	scriptEngine.lua.scriptMouseDragged(x, y, button);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	scriptEngine.lua.scriptMousePressed(x, y, button);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
	scriptEngine.lua.scriptMouseReleased(x, y, button);
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
	
	// set up transforms with new screen size
	transformer.setNewScreenSize(w, h);

	editor.resize(w, h);
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {
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
void ofApp::openFileEvent(int &whichEditor){

	string path =  editor.getEditorFilename(whichEditor);
	ofLogVerbose(PACKAGE) << "editor " << whichEditor << ": opened "
		<< ofFilePath::getFileName(path);
	
	// change the current dir to the scene directory,
	// this allows the lua state to find local files
	if(chdir(ofFilePath::getEnclosingDirectory(path).c_str()) != 0) {
		ofLogError() << "ScriptEngine: couldn't change directory to \"" << path << "\"";
		return;
	}
	
	char currentDir[1024];
	getcwd(currentDir, 1024);
	ofLogVerbose(PACKAGE) << "ScriptEngine: current dir: \"" << currentDir << "\"";
}

//--------------------------------------------------------------
void ofApp::saveFileEvent(int &whichEditor){
	ofLogVerbose(PACKAGE) << "editor " << whichEditor << ": saved "
		<< ofFilePath::getFileName(editor.getEditorFilename(whichEditor));
}

//--------------------------------------------------------------
void ofApp::executeScriptEvent(int &whichEditor){
	scriptEngine.evalString(editor.getText(whichEditor), false);//!editor.isSelection());
}

//--------------------------------------------------------------
void ofApp::evalReplEvent(const string &text) {
	scriptEngine.evalString(text);
	editor.evalReplReturn();
}

//--------------------------------------------------------------
bool ofApp::loadScript(string script) {
	if(script == "") return;

	ofLogVerbose(PACKAGE) << "loading \"" << ofFilePath::getFileName(script) << "\"";
	
	config.script = script;
	config.isPlaylist = false;
	if(!scriptEngine.loadScript(script)) {
		return false;
	}
	
	// load settings
	if(config.isPlaylist) {
		sceneManager.showSceneName(config.showSceneNames);
		sceneManager.gotoScene(0);
		config.playlist = script;
		editor.setHidden(config.hideEditor);
		scriptEngine.unloadScript();
	}
	
	return true;
}

//--------------------------------------------------------------
void ofApp::reloadScript() {
	if(config.script == "") return;
	
	ofLogVerbose(PACKAGE) << "reloading \"" << ofFilePath::getFileName(config.script) << "\"";
	
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
void ofApp::unloadScript() {
	if(config.script == "") return;
	
	ofLogVerbose(PACKAGE) << "unloading \"" << ofFilePath::getFileName(config.script) << "\"";
	
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
bool ofApp::processOscMessage(const ofxOscMessage& message) {

#ifdef DEBUG
	ofLogVerbose(PACKAGE) << "received " << message.getAddress();
#endif
	
	if(message.getAddress() == getOscRootAddress() + "/scene") {
		string name;
		int index;
		if(OscObject::tryString(message, name, 0)) {
			sceneManager.gotoScene(name);
			return true;
		}
		else if(OscObject::tryNumber(message, index, 0)) {
			if(index > -1) {
				sceneManager.gotoScene(index);
			}
			return true;
		}
	}
	
	else if(message.getAddress() == getOscRootAddress() + "/scene/prev") {
		// ignore TouchOsc "button off" events
		if(message.getArgType(0) == OFXOSC_TYPE_FLOAT && message.getArgAsFloat(0) == 0) {
			return true;
		}
		sceneManager.prevScene();
		return true;
	}
	
	else if(message.getAddress() == getOscRootAddress() + "/scene/next") {
		// ignore TouchOsc "button off" events
		if(message.getArgType(0) == OFXOSC_TYPE_FLOAT && message.getArgAsFloat(0) == 0) {
			return true;
		}
		sceneManager.nextScene();
		return true;
	}

	else if(message.getAddress() == getOscRootAddress() + "/file") {
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

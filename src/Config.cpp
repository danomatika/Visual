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
#include "Config.h"

#include "ofApp.h"
#include "options/Options.h"

// PRIVATE
//--------------------------------------------------------------
Config::Config() :
	script(""), isPlaylist(false), playlist(""),
	listeningPort(9990),
	sendingIp("127.0.0.1"), sendingPort(8880),
	baseAddress((string) "/"+PACKAGE),
	notificationAddress(baseAddress+"/notifications"),
	deviceAddress(baseAddress+"/devices"),
	connectionId(0),
	fontFilename(""),
	renderWidth(0), renderHeight(0), fullscreen(false),
	setupAllScenes(true), showSceneNames(true) {}

// PUBLIC
//--------------------------------------------------------------
Config& Config::instance() {
	static Config * pointerToTheSingletonInstance = new Config;
	return *pointerToTheSingletonInstance;
}

//--------------------------------------------------------------
bool Config::parseCommandLine(int argc, char **argv) {
	Options options("  a simple 2d rendering engine", VERSION);
	options.addString("IP", "i", "ip", "IP address to send to (default: 127.0.0.1)");
	options.addInteger("PORT", "p", "port", "IP address to send to (default: 8880)");
	options.addString("LISTENPORT", "l", "listening-port", "IP address to send to (default: 9990)");
	options.addInteger("CONNECTID", "c", "connection-id", "Connection id for notifications (default: 0)");
	options.addSwitch("FULLSCREEN", "f", "fullscreen", "Start in fullscreen?");
	options.addArgument("FILE", "  FILE \tOptional XML config file");
	if(!options.parse(argc, argv)) {
		return false;
	}
	if(options.numArguments() > 0) { // load the config file (if one exists)
		script = ofFilePath::getAbsolutePath(options.getArgumentString(0), false);
		if(ofFilePath::getFileExt(script) != "lua") {
			ofLogError(PACKAGE) << "given script, << \"" << script << " is not a lua file";
			script = "";
			return false;
		}
	}
	if(options.isSet("IP"))         {sendingIp = options.getString("IP");}
	if(options.isSet("PORT"))       {sendingPort = options.getUInt("PORT");}
	if(options.isSet("LISTENPORT")) {listeningPort = options.getUInt("LISTENPORT");}
	if(options.isSet("CONNECTID"))  {connectionId = options.getInt("CONNECTID");}
	if(options.isSet("FULLSCREEN")) {fullscreen = true;}
	return true;
}

//--------------------------------------------------------------
void Config::setup() {

	if(renderWidth == 0) {
		renderWidth = ofGetWidth();
	}
	if(renderHeight == 0) {
		renderHeight = ofGetHeight();
	}

	ofTrueTypeFont::setGlobalDpi(96);
	fontFilename = ofToDataPath(CONFIG_FONT, true);
	functionsFilename = ofToDataPath(CONFIG_FUNCTIONS, true);
	helpFilename = ofToDataPath(CONFIG_HELP, true);
}

//--------------------------------------------------------------
void Config::print() {
	ofLogNotice() << "listening port: " << listeningPort;
	ofLogNotice() << "sending ip: " << sendingIp;
	ofLogNotice() << "sending port: " << sendingPort;
	ofLogNotice() << "base address: " << baseAddress;
	ofLogNotice() << "sending address for notifications: " << notificationAddress;
	ofLogNotice() << "sending address for devices: " << deviceAddress;
	ofLogNotice() << "connection id for notifications: " << connectionId;
	ofLogNotice() << "render size: " << renderWidth << "x" << renderHeight;
	ofLogNotice() << "setup all scenes: " << (setupAllScenes ? "true" : "false");
	ofLogNotice() << "show scene names: " << (showSceneNames ? "true" : "false");
}

//--------------------------------------------------------------
void Config::setRenderSize(unsigned int w, unsigned int h) {
	renderWidth = w;
	renderHeight = h;
}

//--------------------------------------------------------------
void Config::setListeningPort(unsigned int port) {
	if(listeningPort == port) {
		// silently ignore
		return;
	}
	if(port < 1024) {
		ofLogWarning() << "port should be > 1024";
		return;
	}
	Config::instance().listeningPort = port;
	OscReceiver &receiver = Config::instance().oscReceiver;
	if(receiver.isListening()) {
		receiver.stop();
		receiver.setup(port);
		receiver.start();
	}
	else {
		receiver.setup(port);
	}
	ofLogNotice() << "listening port: " << port;
}

//--------------------------------------------------------------
void Config::setSendingIp(string address) {
	if(sendingIp == address) {
		// silently ignore
		return;
	}
	sendingIp = address;
	oscSender.setup(sendingIp, sendingPort);
	ofLogNotice() << "sending ip: " << sendingIp;
}

//--------------------------------------------------------------
void Config::setSendingPort(unsigned int port) {
	if(sendingPort == port) {
		// silently ignore
		return;
	}
	if(port < 1024) {
		ofLogWarning() << "port should be > 1024";
		return;
	}
	sendingPort = port;
	oscSender.setup(sendingIp, sendingPort);
	ofLogNotice() << "sending port: " << port;
}

//--------------------------------------------------------------
void Config::setBaseAddress(string base) {
	baseAddress = base;
	// make sure to update base objects ...
	app->setOscRootAddress(base);
	app->sceneManager.setOscRootAddress(base);
}

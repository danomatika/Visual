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
#include "Config.h"

#include "App.h"
#include "tclap/tclap.h"

//--------------------------------------------------------------
Config& Config::instance() {
	static Config * pointerToTheSingletonInstance = new Config;
	return *pointerToTheSingletonInstance;
}

//--------------------------------------------------------------
bool Config::parseCommandLine(int argc, char **argv) {
	try {
		// the commandline parser
		TCLAP::CommandLine cmd("a simple 2d rendering engine", VERSION);
		
		// options to parse
		// short id, long id, description, required?, default value, short usage type description
		TCLAP::ValueArg<string> ipOpt("i", "ip", (string) "IP address to send to; default is '"+sendingIp+"'", false, sendingIp, "string");
		
		TCLAP::ValueArg<int> portOpt("p","port", (string) "Port to send to; default is '"+ofToString(sendingPort)+"'", false, sendingPort, "int");
	 
		TCLAP::ValueArg<int> inputPortOpt("", "listeningPort", "Listening port; default is '"+ofToString(listeningPort)+"'", false, listeningPort, "int");
		
		TCLAP::ValueArg<int> connectionIdOpt("", "connectionId", "Connection id for notifications; default is '"+ofToString(connectionId)+"'", false, connectionId, "int");
		
		// commands to parse
		// name, description, required?, default value, short usage type description
		TCLAP::UnlabeledValueArg<string> fileCmd("xml", "visual xml file to load", false, "", "file");

		// add args to parser (in reverse order)
		cmd.add(connectionIdOpt);
		cmd.add(inputPortOpt);
		cmd.add(portOpt);
		cmd.add(ipOpt);
		
		// add commands
		cmd.add(fileCmd);
		
		// parse the commandline
		vector<string> line;
		line.push_back(PACKAGE); // set name since argv[0] includes app bundle path
		for(int i = 1; i < argc; ++i) {
			line.push_back((string) argv[i]);
		}
		cmd.parse(line);

		// set the config/playlist file path (if one exists)
		if(fileCmd.getValue() != "") {
			file = ofFilePath::getAbsolutePath(fileCmd.getValue(), false);
			if(ofFilePath::getFileExt(file) != "lua") {
				ofLogError(PACKAGE) << "given config/playlist, << \"" << file << " is not a lua file";
				file = "";
				return false;
			}
		}
		
		// set the variables, may override lua settings
		if(ipOpt.isSet())			sendingIp = ipOpt.getValue();
		if(portOpt.isSet())			sendingPort = portOpt.getValue();
		if(inputPortOpt.isSet())	listeningPort = inputPortOpt.getValue();
		if(connectionIdOpt.isSet())	connectionId = connectionIdOpt.getValue();
	}
	catch(TCLAP::ArgException &e) {	// catch any exceptions
		ofLogError(PACKAGE) << "CommandLine: " << e.error() << " for arg " << e.argId();
		return false;
	}

	return true;
}

//--------------------------------------------------------------
void Config::setup() {
	ofTrueTypeFont::setGlobalDpi(96);
	fontFilename = ofToDataPath(CONFIG_FONT, true);
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
}

void Config::setRenderSize(unsigned int w, unsigned int h) {
	renderWidth = w;
	renderHeight = h;
}

// PRIVATE
//--------------------------------------------------------------
Config::Config() :
	file(""),
	listeningPort(9990),
	sendingIp("127.0.0.1"), sendingPort(8880),
	baseAddress((string) "/"+PACKAGE),
	notificationAddress(baseAddress+"/notifications"),
	deviceAddress(baseAddress+"/devices"),
	connectionId(0),
	fontFilename(""),
	renderWidth(0), renderHeight(0),
	setupAllScenes(true) {}

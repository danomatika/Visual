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

//#include "ofApp.h"
#include "tclap/tclap.h"

Config& Config::instance() {
	static Config * pointerToTheSingletonInstance = new Config;
	return *pointerToTheSingletonInstance;
}

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

		// load the config file (if one exists)
		if(fileCmd.getValue() != "") {
		
			string filePath = ofFilePath::getAbsolutePath(fileCmd.getValue(), false);
			ofLogNotice() << "Config: loading \"" << filePath << "\"";
			
//			ofApp app = (ofApp*) ofGetAppPtr();
//			if(app->getSceneManager().loadXmlFile(filePath)) {
//			
//				// set data path to config file folder
//				visual::Util::setDataPath(visual::Util::getDirPath(filePath));
//		
//				closeXmlFile();
//			}
		}
		
		// set the variables, may override xml settings
		if(ipOpt.isSet())			sendingIp = ipOpt.getValue();
		if(portOpt.isSet())			sendingPort = portOpt.getValue();
		if(inputPortOpt.isSet())	listeningPort = inputPortOpt.getValue();
		if(connectionIdOpt.isSet())	connectionId = connectionIdOpt.getValue();
	}
	catch(TCLAP::ArgException &e) {	// catch any exceptions
		ofLogError() << "CommandLine: " << e.error() << " for arg " << e.argId();
		return false;
	}

	return true;
}

void Config::setup() {
	font.loadFont(fontFilename, CONFIG_FONT_SIZE);
}

void Config::print() {
	ofLog() << "listening port:	" << listeningPort;
	ofLog() << "listening address: " << baseAddress;
	ofLog() << "sending ip: " << sendingIp;
	ofLog() << "sending port: " << sendingPort;
	ofLog() << "sending address for notifications: " << notificationAddress;
	ofLog() << "sending address for devices:       " << deviceAddress;
	ofLog() << "connection id for notifications: " << connectionId;
}

/* ***** PRIVATE ***** */

Config::Config() :
//	XmlObject(""),
	listeningPort(9990),
	sendingIp("127.0.0.1"), sendingPort(8880),
	baseAddress((string) "/"+PACKAGE),
	notificationAddress(baseAddress+"/notifications"),
	deviceAddress(baseAddress+"/devices"),
	connectionId(0),
	fontFilename(ofToDataPath(CONFIG_FONT)) {
	
//	// attach config values to xml attributes
//	addXmlAttribute("port", "listening", XML_TYPE_UINT, &listeningPort);
//	
//	addXmlAttribute("ip", "sending", XML_TYPE_STRING, &sendingIp);
//	addXmlAttribute("port", "sending", XML_TYPE_UINT, &sendingPort);
//	
//	addXmlAttribute("notificationAddress", "osc", XML_TYPE_STRING, &notificationAddress);
//	addXmlAttribute("deviceAddress", "osc", XML_TYPE_STRING, &deviceAddress);
//	addXmlAttribute("connectionId", "osc", XML_TYPE_UINT, &connectionId);
}

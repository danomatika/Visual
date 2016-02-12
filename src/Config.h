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
#pragma once

#include "OscReceiver.h"
#include "ScriptEngine.h"
#include "ResourceManager.h"

#define PACKAGE	"visual"
#define	VERSION	"0.3.0"

#define CONFIG_FONT "fonts/PrintChar21.ttf"
#define CONFIG_FUNCTIONS "functions.lua"
#define CONFIG_HELP "help.lua"

class ofApp;

/// global, per-application instance state variable container class
///
///	implemented as a singleton class following:
///	http://en.wikipedia.org/wiki/Singleton_pattern
///
///	no initialization is needed, just use equt::Config::instance() to access
///	member functions, data
class Config {

	public:

		/// singleton data access, returns a reference to itself
		/// creates a new object on the first call
		static Config& instance();
		
		/// parse the commandline options
		bool parseCommandLine(int argc, char **argv);
		
		/// load resources
		void setup();
		
		/// print current variables
		void print();
		
		/// \section Variables
		
		string script; //< current lua script (playlist or lua run script)
		bool isPlaylist; //< is the script a playlist?
		bool hideEditor; //< hide the editor?
	
		string playlist; //< current playlist, maybe the same as script
		
		unsigned int listeningPort; //< the listening port
		
		string sendingIp; //< ip to send to
		unsigned int sendingPort; //< port to send to
		
		string baseAddress; //< base osc listening/sending address
		string notificationAddress; //< base osc sending address for notifications
		string deviceAddress; //< base osc sending addess for devices
	
		unsigned int connectionId; //< our connection id when sending notifications
		
		string fontFilename; //< font filename
		string functionsFilename; //< lua function overrides
		string helpFilename; //< lua help
		
		unsigned int renderWidth, renderHeight; //< render dimensions
		void setRenderSize(unsigned int w, unsigned int h);
		bool fullscreen; //< start in fullscreen mode?
		
		bool setupAllScenes; //< setup all scenes on load?
		bool showSceneNames; //< show the scene names?

		/// \section Objects
		
		ofPtr<ofApp> app; //< global app pointer
		
		ofxOscSender oscSender; //< global osc sender
		OscReceiver oscReceiver; //< global osc receiver
		
		ScriptEngine scriptEngine; //< global lua scripting engine
		ResourceManager resourceManager; //< global resources
		
		// \section Bindings
		
		void setListeningPort(unsigned int port);
		void setSendingIp(string address);
		void setSendingPort(unsigned int port);
	
		string getBaseAddress() {return baseAddress;}
		void setBaseAddress(string base);
	
	private:
		
		// hide all the constructors, copy functions here
		Config(); // cannot create
		virtual ~Config() {} // cannot destroy
		void operator =(Config& from) {} // not copyable
};

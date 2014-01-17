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
#pragma once

#include "OscReceiver.h"
#include "ResourceManager.h"

#define PACKAGE	"visual"
#define	VERSION	"0.1.0"

#define CONFIG_FONT	"fonts/ATARCC__.TTF"

class App;

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
		
		string file;					///< initial lua config/playlist file
		
		unsigned int listeningPort;		///< the listening port
		
		string sendingIp;				///< ip to send to
		unsigned int sendingPort;		///< port to send to
		
		string baseAddress;				///< base osc listening/sending address
		string notificationAddress;		///< base osc sending address for notifications
		string deviceAddress;			///< base osc sending addess for devices
	
		unsigned int connectionId;		///< our connection id when sending notifications
		
		string fontFilename;			///< font filename
		
		unsigned int renderWidth, renderHeight; ///< render dimensions
		void setRenderSize(unsigned int w, unsigned int h);

		// \section Objects
		
		ofPtr<App> app; ///< global app pointer
		
		ofxOscSender oscSender;       	///< global osc sender
		OscReceiver oscReceiver;		///< global osc receiver
		
		ResourceManager resourceManager; ///< global resources

	private:
		
		// hide all the constructors, copy functions here
		Config();                       // cannot create
		virtual ~Config() {}           	// cannot destroy
		void operator =(Config& from) {}// not copyable
};

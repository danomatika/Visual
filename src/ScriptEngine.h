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
	along with this program. If not, see <http://www.gnu.org/licenses/>.
	
	See https://github.com/danomatika/Visual for documentation

==============================================================================*/
#pragma once

#include "ofxLua.h"

class ofxOscMessage;

class ScriptEngine : private ofxLuaListener {

	public:
	
		ScriptEngine();
	
		bool setup();
		void clear();
		
		/// clears the current lua state, calls scriptExit
		void unloadScript();
		
		/// load a new script
		/// clears the current lua state
		bool loadScript(string script);
		
		/// exit, reinit the lua state, and reload the current script
		bool reloadScript();
		
		/// run a given string
		bool evalString(const string &text, bool reload=false);
		
		/// send an osc message to the lua script
		/// calls the oscReceived lua function
		void sendOsc(const ofxOscMessage& msg);
		
		ofxLua lua;
		
		void setCurrentScript(string script) {currentScript = script;}
		string getCurrentScript() {return currentScript;}
		
	private:
	
		/// lua error callback
		void errorReceived(string& msg);

		string currentScript; //< absolute path to current script
};

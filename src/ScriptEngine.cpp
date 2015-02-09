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
#include "ScriptEngine.h"

#include "Config.h"
#include "ofxOsc.h"

// declare the wrapped modules
extern "C" {
	int luaopen_osc(lua_State* L);
	int luaopen_visual(lua_State* L);
}

//--------------------------------------------------------------
ScriptEngine::ScriptEngine() {
	currentScript = "";
	lua.addListener(this);
}

//--------------------------------------------------------------
bool ScriptEngine::setup() {
	if(!lua.init(true)) {
		ofLogError() << "ScriptEngine: could not init lua";
		return false;
	}
	luaopen_osc(lua); // osc bindings
	luaopen_visual(lua); // visual bindings
	lua.doScript(Config::instance().functionsFilename); // custom functions
	return true;
}

//--------------------------------------------------------------
void ScriptEngine::clear() {
	//if(lua.isValid()) {
		lua.clear();
	//}
	currentScript = "";
}

//--------------------------------------------------------------
void ScriptEngine::unloadScript() {
	lua.scriptExit();
	//if(lua.isValid()) {
		lua.clear();
	//}
}

//--------------------------------------------------------------
bool ScriptEngine::loadScript(string script) {
	ofLogVerbose(PACKAGE) << "ScriptEngine: loading \""
		<< ofFilePath::getFileName(script) << "\"";
	unloadScript();
	if(!setup()) {
		return false;
	}
	currentScript = script;
	
	// change the current dir to the scene directory,
	// this allows the lua state to find local files
	string path = ofFilePath::getEnclosingDirectory(script);
	if(!ofDirectory::doesDirectoryExist(path)) {
		ofLogError() << "ScriptEngine: script dir \"" << path << "\" does not exist";
		return;
	}
	else {
		if(chdir(path.c_str()) != 0) {
			ofLogError() << "ScriptEngine: couldn't change directory to \"" << path << "\"";
			return;
		}
	}
	if(ofGetLogLevel() == OF_LOG_VERBOSE) {
		char currentDir[1024];
		getcwd(currentDir, 1024);
		ofLogVerbose(PACKAGE) << "ScriptEngine: current dir: \"" << currentDir << "\"";
	}
	
	bool ret = lua.doScript(currentScript);
	if(ret) {
		lua.scriptSetup();
	}
	return ret;
}

//--------------------------------------------------------------
bool ScriptEngine::reloadScript() {
	if(currentScript == "") {
		return false;
	}
	ofLogVerbose(PACKAGE) << "ScriptEngine: reloading \""
		<< ofFilePath::getFileName(currentScript) << "\"";
	unloadScript();
	if(!setup()) {
		return false;
	}
	bool ret = lua.doScript(currentScript);
	if(ret) {
		lua.scriptSetup();
	}
	return ret;
}

//--------------------------------------------------------------
bool ScriptEngine::evalString(const string &text, bool reload) {
	if(reload) {
		lua.scriptExit();
		if(!setup()) {
			return false;
		}
	}
	if(!lua.doString(text)) {
		setup(); // reset
		return false;
	}
	if(reload) {
		lua.scriptSetup();
	}
	return true;
}

//--------------------------------------------------------------
void ScriptEngine::sendOsc(const ofxOscMessage& msg) {
	if(!lua.isValid() || !lua.isFunction("oscReceived")) {
		return;
	}
	lua_getglobal(lua, "oscReceived");
	lua.pushobject("ofxOscMessage", new ofxOscMessage(msg));
	if(lua_pcall(lua, 1, 0, 0) != 0) {
		string line = "Error running oscReceived(): " + (string) lua_tostring(lua, -1);
		lua.errorOccurred(line);
	}
}

// PRIVATE
//--------------------------------------------------------------
void ScriptEngine::errorReceived(string& msg) {
	ofLogError(PACKAGE) << msg;
}

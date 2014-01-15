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
#include "ScriptEngine.h"

#include "Config.h"
#include "ofxOsc.h"
#include "ofxLuaBindings.h"
#include "bindings/Bindings.h"

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
	return true;
}

//--------------------------------------------------------------
void ScriptEngine::clear() {
	if(lua.isValid()) {
		lua.clear();
	}
	currentScript = "";
}

//--------------------------------------------------------------
void ScriptEngine::clearScript() {
	lua.scriptExit();
	clear();
}

//--------------------------------------------------------------
bool ScriptEngine::loadScript(string script) {
	ofLogVerbose(PACKAGE) << "ScriptEngine: loading \""
		<< ofFilePath::getFileName(script) << "\"";
	clearScript();
	if(!setup()) {
		return false;
	}
	lua.bind<ofxLuaBindings>();
	lua.bind<Bindings>();
	currentScript = script;
	bool ret = lua.doScript(currentScript);
	if(ret) {
		lua.scriptSetup();
	}
	return ret;
}

//--------------------------------------------------------------
bool ScriptEngine::reloadScript() {
	ofLogVerbose(PACKAGE) << "ScriptEngine: reloading \""
		<< ofFilePath::getFileName(currentScript) << "\"";
	lua.clear();
	if(!setup()) {
		return false;
	}
	lua.bind<ofxLuaBindings>();
	lua.bind<Bindings>();
	bool ret = lua.doScript(currentScript);
	if(ret) {
		lua.scriptSetup();
	}
	return ret;
}

//--------------------------------------------------------------
// calling lua functions with objects:
// http://www.nuclex.org/articles/cxx/1-quick-introduction-to-luabind
void ScriptEngine::sendOsc(const ofxOscMessage& msg) {
	if(!lua.isValid()) {
		return;
	}
	try {
		luabind::call_function<bool>(lua, "oscReceived", boost::ref(msg));
	}
	catch(exception& e) {}
}

// PRIVATE
//--------------------------------------------------------------
void ScriptEngine::errorReceived(string& msg) {
	ofLogError(PACKAGE) << msg;
}

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
#include "Script.h"

#include "App.h"

//--------------------------------------------------------------
Script::Script(string name) : DrawableObject(name),
	filename(""), bLoaded(false) {}

//--------------------------------------------------------------
Script::Script(string name, string filename) :
	DrawableObject(name), filename(filename), bLoaded(false) {}

//--------------------------------------------------------------
bool Script::loadFile(string filename) {
	if(filename == "") {
		filename = this->filename;
	}
	
	string baseName = ofFilePath::getBaseName(filename);
	if(!Config::instance().scriptEngine.loadScript(ofToDataPath(filename))) {
		ofLogWarning() << "Script: \"" << name << "\" couldn't load \""
			<< filename << "\"";
		return false;
	}
	
	// no longer running the config/playlist script
	if(Config::instance().app->bConfigScript) {
		Config::instance().app->bConfigScript = false;
	}
	
	bLoaded = true;
	this->filename = filename;
	ofLogVerbose(PACKAGE) << "Script: loaded \"" << baseName << "\"";

	return true;
}

//--------------------------------------------------------------
void Script::setup() {
	if(!bLoaded) {
		loadFile();
	}
}

//--------------------------------------------------------------
void Script::clear() {
//	filename = "";
	if(bLoaded) {
		Config::instance().scriptEngine.clearScript();
	}
	bLoaded = false;
}

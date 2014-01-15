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
#include "ResourceManager.h"

//--------------------------------------------------------------
ResourceManager::~ResourceManager() {
	clear();
}

//--------------------------------------------------------------
void ResourceManager::clear() {
	clearFonts();
	clearImages();
}

// FONT
//--------------------------------------------------------------
bool ResourceManager::addFont(const string& name, const string& file, unsigned int size) {
	ofPtr<ofTrueTypeFont> f = ofPtr<ofTrueTypeFont>(new ofTrueTypeFont);
	if(!f->loadFont(ofToDataPath(file), size)) {
		return false;
	}
	fonts.insert(pair<string,ofPtr<ofTrueTypeFont> >(name, f));
	return true;
}

//--------------------------------------------------------------
void ResourceManager::removeFont(const string& name) {
	map<string,ofPtr<ofTrueTypeFont> >::iterator iter = fonts.find(name);
	if(iter != fonts.end()) {
		(iter->second).reset();
		fonts.erase(iter);
	}
}

//--------------------------------------------------------------
bool ResourceManager::fontExists(const string& name) {
	map<string,ofPtr<ofTrueTypeFont> >::iterator iter = fonts.find(name);
	if(iter != fonts.end()) {
		return true;
	}
	return false;
}

//--------------------------------------------------------------
ofPtr<ofTrueTypeFont> ResourceManager::getFont(const string& name) {
	map<string,ofPtr<ofTrueTypeFont> >::iterator iter = fonts.find(name);
	if(iter != fonts.end()) {
		return iter->second;
	}
	else {
		return ofPtr<ofTrueTypeFont>(); // NULL
	}
}

//--------------------------------------------------------------
void ResourceManager::clearFonts() {
	map<string,ofPtr<ofTrueTypeFont> >::iterator iter;
	for(iter = fonts.begin(); iter != fonts.end(); iter++) {
		(iter->second).reset();
	}
	fonts.clear();
}

// IMAGE
//--------------------------------------------------------------
bool ResourceManager::addImage(const string& name, const string& file) {
	ofPtr<ofImage> i = ofPtr<ofImage>(new ofImage);
	if(!i->loadImage(ofToDataPath(file))) {
		return false;
	}
	images.insert(pair<string,ofPtr<ofImage> >(name, i));
	return true;
}

//--------------------------------------------------------------
void ResourceManager::removeImage(const string& name) {
	map<string,ofPtr<ofImage> >::iterator iter = images.find(name);
	if(iter != images.end()) {
		(iter->second).reset();
		images.erase(iter);
	}
}

//--------------------------------------------------------------
bool ResourceManager::imageExists(const string& name) {
	map<string,ofPtr<ofImage> >::iterator iter = images.find(name);
	if(iter != images.end()) {
		return true;
	}
	return false;
}

//--------------------------------------------------------------
ofPtr<ofImage> ResourceManager::getImage(const string& name) {
	map<string,ofPtr<ofImage> >::iterator iter = images.find(name);
	if(iter != images.end()) {
		return iter->second;
	}
	else {
		return ofPtr<ofImage>(); // NULL
	}
}

//--------------------------------------------------------------
void ResourceManager::clearImages() {
	map<string,ofPtr<ofImage> >::iterator iter;
	for(iter = images.begin(); iter != images.end(); iter++) {
		(iter->second).reset();
	}
	images.clear();
}

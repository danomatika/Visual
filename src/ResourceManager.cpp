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
//void ResourceManager::update() {
//	map<string,ofPtr<ofVideoPlayer> >::iterator iter;
//	for(iter = videos.begin(); iter != videos.end(); iter++) {
//		(iter->second)->update();
//	}
//}

//--------------------------------------------------------------
void ResourceManager::clear() {
	clearFonts();
	clearImages();
	clearVideos();
}

// FONT
//--------------------------------------------------------------
bool ResourceManager::addFont(const string& name, unsigned int size, string file) {
	
	map<string,FontSet>::iterator nameIter = fonts.find(name);
	if(nameIter != fonts.end()) {
		
		// existing font
		FontSet &fontSet = (nameIter->second);
		map<unsigned int,ofPtr<ofTrueTypeFont> >::iterator sizeIter = fontSet.fonts.find(size);
		if(sizeIter != fontSet.fonts.end()) { // already exists at this size
			return true;
		}
		else {
			
			// add new size
			if(file == "") { // reuse filename
				file = fontSet.filename;
			}
			ofPtr<ofTrueTypeFont> f = ofPtr<ofTrueTypeFont>(new ofTrueTypeFont);
			if(!f->loadFont(ofToDataPath(file), size)) {
				return false;
			}
			fontSet.fonts.insert(pair<unsigned int,ofPtr<ofTrueTypeFont> >(size, f));
		}
	}
	else { // new font
		ofPtr<ofTrueTypeFont> f = ofPtr<ofTrueTypeFont>(new ofTrueTypeFont);
		if(!f->loadFont(ofToDataPath(file), size)) {
			return false;
		}
		FontSet fontSet;
		fontSet.fonts.insert(pair<unsigned int,ofPtr<ofTrueTypeFont> >(size, f));
		fonts.insert(pair<string,FontSet>(name, fontSet));
	}
	
	return true;
}

//--------------------------------------------------------------
void ResourceManager::removeFont(const string& name, unsigned int size) {
	map<string,FontSet>::iterator nameIter = fonts.find(name);
	if(nameIter != fonts.end()) {
		FontMap &fontMap = (nameIter->second).fonts;
		map<unsigned int,ofPtr<ofTrueTypeFont> >::iterator sizeIter = fontMap.find(size);
		if(sizeIter != fontMap.end()) {
			(sizeIter->second).reset();
			fontMap.erase(sizeIter);
		}
	}
}

//--------------------------------------------------------------
bool ResourceManager::fontExists(const string& name, unsigned int size) {
	map<string,FontSet>::iterator nameIter = fonts.find(name);
	if(nameIter != fonts.end()) {
		FontMap &fontMap = (nameIter->second).fonts;
		map<unsigned int,ofPtr<ofTrueTypeFont> >::iterator sizeIter = fontMap.find(size);
		if(sizeIter != fontMap.end()) {
			return true;
		}
	}
	return false;
}

//--------------------------------------------------------------
ofPtr<ofTrueTypeFont> ResourceManager::getFont(const string& name, unsigned int size) {
	map<string,FontSet>::iterator nameIter = fonts.find(name);
	if(nameIter != fonts.end()) {
		FontMap &fontMap = (nameIter->second).fonts;
		map<unsigned int,ofPtr<ofTrueTypeFont> >::iterator sizeIter = fontMap.find(size);
		if(sizeIter != fontMap.end()) {
			return sizeIter->second;
		}
	}
	return ofPtr<ofTrueTypeFont>(); // NULL
}

//--------------------------------------------------------------
void ResourceManager::clearFonts() {
	map<string,FontSet>::iterator nameIter;
	for(nameIter = fonts.begin(); nameIter != fonts.end(); nameIter++) {
		FontMap &fontMap = (nameIter->second).fonts;
		map<unsigned int,ofPtr<ofTrueTypeFont> >::iterator sizeIter;
		for(sizeIter = fontMap.begin(); sizeIter != fontMap.end(); sizeIter++) {
			(sizeIter->second).reset();
		}
		fontMap.clear();
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

//--------------------------------------------------------------
bool ResourceManager::addVideo(const string& name, const string& file) {
	ofPtr<ofVideoPlayer> v = ofPtr<ofVideoPlayer>(new ofVideoPlayer);
	if(!v->loadMovie(ofToDataPath(file))) {
		return false;
	}
	videos.insert(pair<string,ofPtr<ofVideoPlayer> >(name, v));
	return true;
}

//--------------------------------------------------------------
void ResourceManager::removeVideo(const string& name) {
	map<string,ofPtr<ofVideoPlayer> >::iterator iter = videos.find(name);
	if(iter != videos.end()) {
		(iter->second).reset();
		videos.erase(iter);
	}
}

//--------------------------------------------------------------
bool ResourceManager::videoExists(const string& name) {
	map<string,ofPtr<ofVideoPlayer> >::iterator iter = videos.find(name);
	if(iter != videos.end()) {
		return true;
	}
	return false;
}

//--------------------------------------------------------------
ofPtr<ofVideoPlayer> ResourceManager::getVideo(const string& name) {
	map<string,ofPtr<ofVideoPlayer> >::iterator iter = videos.find(name);
	if(iter != videos.end()) {
		return iter->second;
	}
	else {
		return ofPtr<ofVideoPlayer>(); // NULL
	}
}

//--------------------------------------------------------------
void ResourceManager::clearVideos() {
	map<string,ofPtr<ofVideoPlayer> >::iterator iter;
	for(iter = videos.begin(); iter != videos.end(); iter++) {
		(iter->second).reset();
	}
	videos.clear();
}

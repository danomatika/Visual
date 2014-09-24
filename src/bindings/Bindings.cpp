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
#include "Bindings.h"

#include "App.h"
#include "Scene.h"
#include "Objects.h"

#include <luabind/adopt_policy.hpp>

//--------------------------------------------------------------
void print(const string& message) {
	ofLogNotice() << message;
}

//--------------------------------------------------------------
Config* getConfig() {
	return &Config::instance();
}

//--------------------------------------------------------------
void configSetBaseAddress(Config *config, string base) {
	config->baseAddress = base;
	// make sure to update base objects ...
	config->app->setOscRootAddress(base);
	config->app->sceneManager.setOscRootAddress(base);
}

//--------------------------------------------------------------
float getRenderWidth() {
	return Config::instance().renderWidth;
}

float getRenderHeight() {
	return Config::instance().renderHeight;
}

//--------------------------------------------------------------
void addScene(Scene *scene) {
	Config::instance().app->sceneManager.addScene(scene);
}

Scene* getCurrentScene() {
	return Config::instance().app->sceneManager.getCurrentScene();
}

//--------------------------------------------------------------
void setListeningPort(unsigned int port) {
	Config::instance().setListeningPort(port);
}

void setSendingIp(string address) {
	Config::instance().setSendingIp(address);
}

void setSendingPort(unsigned int port) {
	Config::instance().setSendingPort(port);
}

//--------------------------------------------------------------
// luabind registration
//
// transferring ownership reference:
// http://lua.2524044.n2.nabble.com/How-to-create-objects-in-Lua-and-transfer-ownership-to-C-td7581800.html
luabind::scope Bindings::registerBindings() {
		
	using namespace luabind;
	
	return
		
		///////////////////////////////
		/// \section Utils
		def("print", &print), // print override
		
		///////////////////////////////
		/// \section Config reatime functions
		def("setListeningPort", &setListeningPort),
		def("setSendingAddress", &setSendingIp),
		def("setSendingPort", &setSendingPort),
		
		///////////////////////////////
		/// \section Config.h
		
		def("getConfig", &getConfig), // always owned by C++
		
		// this is a dummy class, you shouldn't be able to create it directly,
		// it's only for access to the config variables
		class_<Config>("Config")
			.def("print", &Config::print)
			.def_readwrite("isPlaylist", &Config::isPlaylist)
			.def_readwrite("listeningPort", &Config::listeningPort)
			.def_readwrite("sendingIp", &Config::sendingIp)
			.def_readwrite("sendingPort", &Config::sendingPort)
			.property("baseAddress", &Config::baseAddress, &configSetBaseAddress)
			.def_readwrite("notificationAddress", &Config::notificationAddress)
			.def_readwrite("deviceAddress", &Config::deviceAddress)
			.def_readwrite("connectionId", &Config::connectionId)
			.def("setRenderSize", &Config::setRenderSize)
			.def_readonly("renderWidth", &Config::renderWidth)
			.def_readonly("renderHeight", &Config::renderHeight)
			.def_readwrite("fullscreen", &Config::fullscreen)
			.def_readwrite("setupAllScenes", &Config::setupAllScenes)
			.def_readwrite("showSceneNames", &Config::showSceneNames),
			
		def("getRenderWidth", &getRenderWidth),
		def("getRenderHeight", &getRenderHeight),
		
		///////////////////////////////
		/// \section Scene.h
		
		def("addScene", &addScene, adopt(_1)), // transfer ownership to C++
		def("getCurrentScene", &getCurrentScene),
		
		class_<Scene>("Scene")
			.def(constructor<string>())
			
			.def("add", &Scene::addObject, adopt(_2)) // transfer to C++
			.def("remove", (void(Scene::*)(DrawableObject*)) &Scene::removeObject)
			.def("clear", &Scene::clearObjects)
			.def("setup", &Scene::setup)
			.def("draw", &Scene::draw)
			
			.property("slideshow", &Scene::getSlideshow, &Scene::setSlideshow)
			.def("nextObject", &Scene::nextObject)
			.def("prevObject", &Scene::prevObject)
			.def("gotoObject", (void(Scene::*)(unsigned int)) &Scene::gotoObject)
			.def("gotoObject", (void(Scene::*)(string)) &Scene::gotoObject)
			
			.def("getName", &Scene::getName)
			.property("name", &Scene::getName)
			.property("fps", &Scene::getFps, &Scene::setFps)
			.property("background", &Scene::getBackground, &Scene::setBackground),
		
		///////////////////////////////
		/// \section DrawableObject.h
		
		class_<DrawableObject>("DrawableObject")
			.def(constructor<string>())
			
			.def("draw", (void(DrawableObject::*)(void)) &DrawableObject::draw)
			.def("draw", (void(DrawableObject::*)(int,int)) &DrawableObject::draw)
			.def("draw", (void(DrawableObject::*)(int,int,unsigned int,unsigned int)) &DrawableObject::draw)
			.def("clear", &DrawableObject::clear)
			
			.property("visible", &DrawableObject::getVisible, &DrawableObject::setVisible)
			.property("color", &DrawableObject::getColor, &DrawableObject::setColor)
			.def("setSize", &DrawableObject::setSize)
			.property("name", &DrawableObject::getName)
			.property("type", &DrawableObject::getType),
			
		class_<DrawableFrame, DrawableObject>("DrawableFrame")
			.def(constructor<string>())
			
			.def("draw", (void(DrawableFrame::*)(void)) &DrawableFrame::draw)
			.def("draw", (void(DrawableFrame::*)(int,int)) &DrawableFrame::draw)
			.def("draw", (void(DrawableFrame::*)(int,int,unsigned int,unsigned int)) &DrawableFrame::draw)
			.def("clear", &DrawableFrame::clear)
			
			.property("visible", &DrawableFrame::getVisible, &DrawableFrame::setVisible)
			.property("color", &DrawableFrame::getColor, &DrawableFrame::setColor)
			.def("setSize", &DrawableFrame::setSize)
			.property("name", &DrawableFrame::getName)
			.property("type", &DrawableFrame::getType),
		
		///////////////////////////////
		/// \section Bitmap.h
		
		class_<Bitmap, bases<DrawableFrame, DrawableObject> >("Bitmap")
			.def(constructor<string,unsigned int,unsigned int>())
			.def(constructor<unsigned int,unsigned int,unsigned int>())
			
			.def("draw", (void(Bitmap::*)(void)) &Bitmap::draw)
			.def("draw", (void(Bitmap::*)(int,int)) &Bitmap::draw)
			.def("draw", (void(Bitmap::*)(int,int,unsigned int,unsigned int)) &Bitmap::draw)
			.def("clear", &Bitmap::clear)
			
			.property("bitmap", (void(Bitmap::*)(string)) &Bitmap::getBitmap, (void(Bitmap::*)(string)) &Bitmap::setBitmap)
			.property("visible", &Bitmap::getVisible, &Bitmap::setVisible)
			.property("color", &Bitmap::getColor, &Bitmap::setColor)
			.property("position", &Bitmap::getPos, &Bitmap::setPos)
			.def("setSize", &Bitmap::setSize)
			.property("width", &Bitmap::getWidth, &Bitmap::setWidth)
			.property("height", &Bitmap::getHeight, &Bitmap::setHeight)
			.property("filled", &Bitmap::getFilled, &Bitmap::setFilled)
			.property("center", &Bitmap::getDrawFromCenter, &Bitmap::setDrawFromCenter)
			.property("name", &Bitmap::getName)
			.property("type", &Bitmap::getType),
			
		///////////////////////////////
		/// \section Image.h
		
		class_<Image, bases<DrawableFrame, DrawableObject> >("Image")
			.def(constructor<string>())
			.def(constructor<string,string>())
			.def(constructor<unsigned int,string>())
			.def("loadFile", &Image::loadFile)
			
			.def("setup", &Image::setup)
			.def("draw", (void(Image::*)(void)) &Image::draw)
			.def("draw", (void(Image::*)(int,int)) &Image::draw)
			.def("draw", (void(Image::*)(int,int,unsigned int,unsigned int)) &Image::draw)
			.def("clear", &Image::clear)
			
			.def("getImage", &Image::getImage)
			.property("image", &Image::getImage)
			.def("isLoaded", &Image::isLoaded)
			.property("loaded", &Image::isLoaded)
			.def("getFilename", &Image::getFilename)
			.property("filename", &Image::getFilename)
			.property("color", &Image::getColor, &Image::setColor)
			.property("visible", &Image::getVisible, &Image::setVisible)
			.property("position", &Image::getPos, &Image::setPos)
			.def("setSize", &Image::setSize)
			.property("width", &Image::getWidth, &Image::setWidth)
			.property("height", &Image::getHeight, &Image::setHeight)
			.property("center", &Image::getDrawFromCenter, &Image::setDrawFromCenter)
			.property("name", &Image::getName)
			.property("type", &Image::getType),
			
		///////////////////////////////
		/// \section Line.h
		
		class_<Line, DrawableObject>("Line")
			.def(constructor<string>())
			.def(constructor<string,int,int,int,int>())
			.def(constructor<string,ofPoint&,ofPoint&>())
			
			.def("draw", &Line::draw)
			
			.property("color", &Line::getColor, &Line::setColor)
			.property("visible", &Line::getVisible, &Line::setVisible)
			.property("position1", &Line::getPos1, &Line::setPos1)
			.property("position2", &Line::getPos2, &Line::setPos2)
			.property("name", &Line::getName)
			.property("type", &Line::getType),
			
		///////////////////////////////
		/// \section Pixel.h
		
		class_<Pixel, DrawableObject>("Pixel")
			.def(constructor<string>())
			.def(constructor<string,int,int>())
			
			.def("draw", &Pixel::draw)
			
			.property("position", &Pixel::getPos, &Pixel::setPos)
			.property("color", &Pixel::getColor, &Pixel::setColor)
			.property("visible", &Pixel::getVisible, &Pixel::setVisible)
			.property("name", &Pixel::getName)
			.property("type", &Pixel::getType),
			
		///////////////////////////////
		/// \section Rectangle.h
		
		class_<Rectangle, DrawableObject>("Rectangle")
			.def(constructor<string>())
			.def(constructor<string,int,int,unsigned int,unsigned int>())
			.def(constructor<string,ofPoint&,unsigned int,unsigned int>())
			
			.def("draw", &Rectangle::draw)
			
			.property("color", &Rectangle::getColor, &Rectangle::setColor)
			.property("visible", &Rectangle::getVisible, &Rectangle::setVisible)
			.property("position", &Rectangle::getPos, &Rectangle::setPos)
			.def("setSize", &Rectangle::setSize)
			.property("width", &Rectangle::getWidth, &Rectangle::setWidth)
			.property("height", &Rectangle::getHeight, &Rectangle::setHeight)
			.property("filled", &Rectangle::getFilled, &Rectangle::setFilled)
			.property("center", &Rectangle::getDrawFromCenter, &Rectangle::setDrawFromCenter)
			.property("name", &Rectangle::getName)
			.property("type", &Rectangle::getType),
		
		///////////////////////////////
		/// \section Script.h
		
		class_<Script, DrawableObject>("Script")
			.def(constructor<string>())
			.def(constructor<string,string>())
			
			.def("setup", &Script::setup)
			.def("isLoaded", &Script::isLoaded)
			.property("loaded", &Script::isLoaded)
			.def("getFilename", &Script::getFilename)
			.property("filename", &Script::getFilename)
			.property("name", &Script::getName)
			.property("type", &Script::getType),
		
		///////////////////////////////
		/// \section Sprite.h
		
		class_<Sprite, DrawableObject>("Sprite")
			.def(constructor<string>())
			
			.def("add", &Sprite::addFrame, adopt(_2)) // transfer to C++
			.def("remove", &Sprite::removeFrame)
			.def("clear", &Sprite::clearFrames)
			
			.def("nextFrame", &Sprite::nextFrame)
			.def("prevFrame", &Sprite::prevFrame)
			.def("gotoFrame", (void(Sprite::*)(unsigned int)) &Sprite::gotoFrame)
			.def("gotoFrame", (void(Sprite::*)(string)) &Sprite::gotoFrame)
			.def("getCurrentFrame", &Sprite::getCurrentFrame)
			.property("frame", &Sprite::getCurrentFrame, (void(Sprite::*)(unsigned int)) &Sprite::gotoFrame)
			
			.def("setup", &Sprite::setup)
			.def("draw", &Sprite::draw)
			
			.property("visible", &Sprite::getVisible, &Sprite::setVisible)
			.property("position", &Sprite::getPos, &Sprite::setPos)
			.def("setSize", &Sprite::setSize)
			.property("width", &Sprite::getWidth, &Sprite::setWidth)
			.property("height", &Sprite::getHeight, &Sprite::setHeight)
			.def("setAnimation", &Sprite::setAnimation)
			.property("animate", &Sprite::getAnimate, &Sprite::setAnimate)
			.property("loop", &Sprite::getLoop, &Sprite::setLoop)
			.property("pingpong", &Sprite::getPingPong, &Sprite::setPingPong)
			.property("center", &Sprite::getDrawFromCenter, &Sprite::setDrawFromCenter)
			.property("overlay", &Sprite::getDrawAllLayers, &Sprite::setDrawAllLayers)
			.property("name", &Sprite::getName)
			.property("type", &Sprite::getType),
			
		///////////////////////////////
		/// \section Text.h
		
		class_<Text, DrawableObject>("Text")
			.def(constructor<string>())
			.def(constructor<string,string>())
			.def(constructor<string,unsigned int>())
			.def(constructor<string,string,unsigned int>())
			
			.def("setup", &Text::setup)
			.def("draw", (void(Text::*)(void)) &Text::draw)
			.def("draw", (void(Text::*)(int,int)) &Text::draw)
			
			.def("getFont", &Text::getFont)
			.property("font", &Text::getFont)
			.def("isFontLoaded", &Text::isFontLoaded)
			.property("fontLoaded", &Text::isFontLoaded)
			.def("getFontFilename", &Text::getFontFilename)
			.property("fontFilename", &Text::getFontFilename)
			.def("getFontSize", &Text::getFontSize)
			.property("fontSize", &Text::getFontSize)
			.property("color", &Text::getColor, &Text::setColor)
			.property("visible", &Text::getVisible, &Text::setVisible)
			.property("position", &Text::getPos, &Text::setPos)
			.property("text", &Text::getText, &Text::setText)
			.property("center", &Text::getDrawFromCenter, &Text::setDrawFromCenter)
			.property("name", &Text::getName)
			.property("type", &Text::getType),
			
		///////////////////////////////
		/// \section Video.h
		
		class_<Video, DrawableObject>("Video")
			.def(constructor<string>())
			.def(constructor<string,string>())
			//.def(constructor<string,Video&>())
			.def("loadFile", &Video::loadFile)
			
			.def("setup", &Video::setup)
			.def("draw", (void(Video::*)(void)) &Video::draw)
			.def("draw", (void(Video::*)(int,int)) &Video::draw)
			.def("draw", (void(Video::*)(int,int,unsigned int,unsigned int)) &Video::draw)
			.def("clear", &Video::clear)
			
			.def("getVideo", &Video::getVideo)
			.def("isLoaded", &Video::isLoaded)
			.property("loaded", &Video::isLoaded)
			.property("video", &Video::getVideo)
			.property("play", &Video::getPlay, &Video::setPlay)
			.property("volume", &Video::getVolume, &Video::setVolume)
			.property("speed", &Video::getSpeed, &Video::setSpeed)
			.property("loop", &Video::getLoop, &Video::setLoop)
			.def("getFilename", &Video::getFilename)
			.property("filename", &Video::getFilename)
			.property("color", &Video::getColor, &Video::setColor)
			.property("visible", &Video::getVisible, &Video::setVisible)
			.property("position", &Video::getPos, &Video::setPos)
			.def("setSize", &Video::setSize)
			.property("width", &Video::getWidth, &Video::setWidth)
			.property("height", &Video::getHeight, &Video::setHeight)
			.property("center", &Video::getDrawFromCenter, &Video::setDrawFromCenter)
			.property("name", &Video::getName)
			.property("type", &Video::getType)
	;
}

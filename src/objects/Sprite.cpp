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
#include "Sprite.h"

//--------------------------------------------------------------
SpriteFrame::SpriteFrame(DrawableObject *object): frameTime(100), o(object) {

//	// add variables to Xml
//	o->addXmlAttribute("frametime", o->getType(), XML_TYPE_UINT, &frameTime);
//	
//	// detach unneeded variables from Xml
//	o->removeXmlAttribute("name", o->getType());
}

//--------------------------------------------------------------
SpriteFrame::~SpriteFrame() {
	if(o) delete o;
}

//--------------------------------------------------------------
Sprite::Sprite(string name, string parentOscAddress) :
	DrawableObject("sprite", name, parentOscAddress),
	pos(0, 0), width(0), height(0), bAnimate(true), bLoop(true), bPingPong(true),
	bDrawFromCenter(false), bDrawAllLayers(false),
	currentFrame(0), timestamp(0), bForward(true) {
	
//	// add variables to Xml
//	addXmlAttribute("x", "position", XML_TYPE_FLOAT, &pos.x);
//	addXmlAttribute("y", "position", XML_TYPE_FLOAT, &pos.y);
//	addXmlAttribute("width", "size", XML_TYPE_UINT, &width);
//	addXmlAttribute("height", "size", XML_TYPE_UINT, &height);
//	addXmlAttribute("animate", "animation", XML_TYPE_BOOL, &bAnimate);
//	addXmlAttribute("loop", "animation", XML_TYPE_BOOL, &bLoop);
//	addXmlAttribute("pingpong", "animation", XML_TYPE_BOOL, &bPingPong);
//	addXmlAttribute("yesno", "center", XML_TYPE_BOOL, &bDrawFromCenter);
//	addXmlAttribute("yesno", "overlay", XML_TYPE_BOOL, &bDrawAllLayers);
//
//	// detach unneeded variables from Xml
//	removeXmlElement("color");

	timestamp = ofGetElapsedTimeMillis();
}

//--------------------------------------------------------------
Sprite::~Sprite() {
	clear();
}

//--------------------------------------------------------------
void Sprite::addFrame(SpriteFrame* frame) {
	if(frame == NULL) {
		ofLogError() << "Sprite: cannot add NULL frame";
		return;
	}

//	addXmlObject(frame->getObject());
	frames.push_back(frame);
}

//--------------------------------------------------------------
void Sprite::removeFrame(SpriteFrame* frame) {
	if(frame == NULL) {
		ofLogError() << "Sprite: cannot remove NULL frame";
		return;
	}

	vector<SpriteFrame*>::iterator iter;
	iter = find(frames.begin(), frames.end(), frame);
	if(iter != frames.end()) {
//		removeXmlObject((*iter)->getObject());
		delete (*iter);
		frames.erase(iter);
	}
}

//--------------------------------------------------------------
void Sprite::clear() {
	/// delete all frames
	for(unsigned int i = 0; i < frames.size(); ++i) {
		SpriteFrame* f = frames.at(i);
		delete f;
	}
	frames.clear();
}

//--------------------------------------------------------------
void Sprite::nextFrame() {
	if(frames.size() < 2) {
		return;
	}

	currentFrame++;

	if(currentFrame >= (int) frames.size()) {
		if(bPingPong) {
			bForward = false;
			currentFrame = frames.size()-2;
		}
		else {
			currentFrame = 0;
		}
	}
}

//--------------------------------------------------------------
void Sprite::prevFrame() {
	if(frames.size() < 2) {
		return;
	}

	currentFrame--;

	if(currentFrame < 0) {
		if(bPingPong) {
			bForward = true;
			currentFrame = 1;
		}
		else {
			currentFrame = frames.size()-1;
		}
	}
}

//--------------------------------------------------------------
void Sprite::gotoFrame(unsigned int num) {
	if(currentFrame >= (int) frames.size()) {
		ofLogWarning() << "Sprite: Cannot goto frame num " << num
			<< ", index out of range" << endl;
		return;
	}
	currentFrame = num;
}

//--------------------------------------------------------------
void Sprite::gotoFrame(string name) {
	for(unsigned int i = 0; i < frames.size(); ++i) {
		if(name == frames.at(i)->getName()) {
			currentFrame = i;
			return;
		}
	}
}

//--------------------------------------------------------------
void Sprite::setup() {
	for(unsigned int i = 0; i < frames.size(); ++i) {
		frames[i]->setup();
		if(width != 0 && height != 0) {
			frames[i]->getObject()->setSize(width, height);
		}
	}
}

//--------------------------------------------------------------
void Sprite::draw() {
	if(frames.empty()) {
		return;
	}

	// animate frames?
	if(bAnimate) {
	
		// go to next frame if time has elapsed
		if(ofGetElapsedTimeMillis() - timestamp > frames.at(currentFrame)->getFrameTime()) {
			if(bForward) {
				nextFrame();
			}
			else {
				prevFrame();
			}
			timestamp = ofGetElapsedTimeMillis();
		}
	}

	// draw frame(s)?
	if(bVisible) {
		if(bDrawAllLayers) {
			for(unsigned int i = 0; i < frames.size(); ++i) {
				SpriteFrame* f = frames.at(i);
				f->draw(pos.x, pos.y);
			}
		}
		else if(currentFrame >= 0 && currentFrame < (int) frames.size()) {
			SpriteFrame* f = frames.at(currentFrame);
			f->draw(pos.x, pos.y);
		}
	}
}

//--------------------------------------------------------------
//void Sprite::resizeIfNecessary() {
//	if(width != 0 && height != 0) {
//		for(unsigned int i = 0; i < frames.size(); ++i) {
//			SpriteFrame* f = frames.at(i);
//			f->getObject()->setSize(width, height);
//		}
//	}
//}

//--------------------------------------------------------------
void Sprite::setDrawFromCenter(bool yesno) {
	bDrawFromCenter = yesno;
	for(unsigned int i = 0; i < frames.size(); ++i) {
		SpriteFrame* f = frames.at(i);
		f->getObject()->setDrawFromCenter(bDrawFromCenter);
	}
}

////--------------------------------------------------------------
//bool Sprite::readXml(TiXmlElement* e)
//{
//	string name;
//
//	TiXmlElement* child = e->FirstChildElement();
//	while(child != NULL)
//	{
//		if(child->ValueStr() == "bitmap")
//		{
//			name = Xml::getAttrString(child, "name", visual::Util::toString(frames.size()));
//			LOG_DEBUG << "Sprite: Loading bitmap \"" << name << "\"" << std::endl;
//
//			SpriteFrame* f = new SpriteFrame(new Bitmap(name, oscRootAddress));
//			if(f->getObject()->loadXml(child))
//			{
//				addFrame(f);
//			}
//		}
//		else if(child->ValueStr() == "image")
//		{
//			name = Xml::getAttrString(child, "name", visual::Util::toString(frames.size()));
//			LOG_DEBUG << "Sprite: Loading image \"" << name << "\"" << std::endl;
//
//			SpriteFrame* f = new SpriteFrame(new Image(name, oscRootAddress));
//			if(f->getObject()->loadXml(child))
//			{
//				addFrame(f);
//			}
//		}
//
//		child = child->NextSiblingElement();
//	}
//
//	// init all loaded bitmaps draw settings
//	setDrawFromCenter(bDrawFromCenter);
//
//	return true;
//}

//--------------------------------------------------------------
bool Sprite::processOscMessage(const ofxOscMessage& message) {
	
	// call the base class
	if(DrawableObject::processOscMessage(message)) {
		return true;
	}


	if(message.getAddress() == oscRootAddress + "/position") {
		Util::tryNumber(message, pos.x, 0);
		Util::tryNumber(message, pos.y, 1);
		return true;
	}
	else if(message.getAddress() == oscRootAddress + "/position/x") {
		Util::tryNumber(message, pos.x, 0);
		return true;
	}
	else if(message.getAddress() == oscRootAddress + "/position/y") {
		Util::tryNumber(message, pos.y, 0);
		return true;
	}
	
	
	else if(message.getAddress() == oscRootAddress + "/size") {
		Util::tryNumber(message, width, 0);
		Util::tryNumber(message, height, 1);
//		resizeIfNecessary();
		return true;
	}
	else if(message.getAddress() == oscRootAddress + "/size/width") {
		Util::tryNumber(message, width, 0);
//		resizeIfNecessary();
		return true;
	}
	else if(message.getAddress() == oscRootAddress + "/size/height") {
		Util::tryNumber(message, height, 0);
//		resizeIfNecessary();
		return true;
	}

	else if(message.getAddress() == oscRootAddress + "/frame") {
		unsigned int frame = 0;
		Util::tryNumber(message, frame, 0);
		gotoFrame(frame);
		return true;
	}

	else if(message.getAddress() == oscRootAddress + "/center") {
		bool b = bDrawFromCenter;
		if(Util::tryBool(message, b, 0)) {
			setDrawFromCenter(b);
		}
		return true;
	}


	else if(message.getAddress() == oscRootAddress + "/animate") {
		Util::tryBool(message, bAnimate, 0);
		return true;
	}
	
	else if(message.getAddress() == oscRootAddress + "/overlay") {
		Util::tryBool(message, bDrawFromCenter, 0);
		return true;
	}


	return false;
}

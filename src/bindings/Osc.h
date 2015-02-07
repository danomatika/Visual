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
#include "Config.h"

//--------------------------------------------------------------
static void sendBang(string address) {
	ofxOscMessage msg;
	msg.setAddress(address);
	Config::instance().oscSender.sendMessage(msg);
}

//--------------------------------------------------------------
static void sendFloat(string address, float f) {
	ofxOscMessage msg;
	msg.setAddress(address);
	msg.addFloatArg(f);
	Config::instance().oscSender.sendMessage(msg);
}

//--------------------------------------------------------------
static void sendInt(string address, int i) {
	ofxOscMessage msg;
	msg.setAddress(address);
	msg.addIntArg(i);
	Config::instance().oscSender.sendMessage(msg);
}

//--------------------------------------------------------------
static void sendString(string address, string s) {
	ofxOscMessage msg;
	msg.setAddress(address);
	msg.addStringArg(s);
	Config::instance().oscSender.sendMessage(msg);
}

//--------------------------------------------------------------
static void sendMessage(ofxOscMessage& msg) {
	Config::instance().oscSender.sendMessage(msg);
}

//--------------------------------------------------------------
static void sendBundle(ofxOscBundle& bundle) {
	Config::instance().oscSender.sendBundle(bundle);
}

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

#include "Config.h"

//--------------------------------------------------------------
static void send(string address) {
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

//--------------------------------------------------------------
// dummy class for argument types
struct OscArgType {};

//--------------------------------------------------------------
// luabind registration
luabind::scope Bindings::registerOsc() {
		
	using namespace luabind;
	
	return
				
		///////////////////////////////
		/// \section Osc
		
		def("send", &send),
		def("sendFloat", &sendFloat),
		def("sendInt", &sendInt),
		def("sendString", &sendString),
		def("sendMessage", &sendMessage),
		def("sendBundle", &sendBundle),
			
		///////////////////////////////
		/// \section Message
		
		class_<OscArgType>("TYPE")
			.enum_("argType") [
				value("NONE", OFXOSC_TYPE_NONE),
				value("INT", OFXOSC_TYPE_INT32),
				value("FLOAT", OFXOSC_TYPE_FLOAT),
				value("STRING", OFXOSC_TYPE_STRING),
				value("BLOB", OFXOSC_TYPE_BLOB),
				value("BUNDLE", OFXOSC_TYPE_BUNDLE),
				value("OUTOFBOUNDS", OFXOSC_TYPE_INDEXOUTOFBOUNDS)
			],
		
		class_<ofxOscMessage>("Message")
			.def(constructor<>())
			.def("clear", &ofxOscMessage::clear)
			.def("setAddress", &ofxOscMessage::setAddress)
			.def("getAddress", &ofxOscMessage::getAddress)
			.def("getRemoteIp", &ofxOscMessage::getRemoteIp)
			.def("getRemotePort", &ofxOscMessage::getRemotePort)
			.def("setRemote", &ofxOscMessage::setRemoteEndpoint)
			.def("getType", &ofxOscMessage::getArgType)
			.def("getTypeName", &ofxOscMessage::getArgTypeName)
			.def("getInt", &ofxOscMessage::getArgAsInt32)
			.def("getFloat", &ofxOscMessage::getArgAsFloat)
			.def("getString", &ofxOscMessage::getArgAsString)
			.def("addInt", &ofxOscMessage::addIntArg)
			.def("addFloat", &ofxOscMessage::addFloatArg)
			.def("addString", &ofxOscMessage::addStringArg)
			.property("numArgs", &ofxOscMessage::getNumArgs),
			
		///////////////////////////////
		/// \section Bundle
		
		class_<ofxOscBundle>("Bundle")
			.def(constructor<>())
			.def("clear", &ofxOscBundle::clear)
			.def("addMessage", &ofxOscBundle::addMessage)
			.def("addBundle", &ofxOscBundle::addBundle)
			.def("getMessage", &ofxOscBundle::getMessageAt)
			.def("getBundle", &ofxOscBundle::getBundleAt)
			.property("numMessages", &ofxOscBundle::getMessageCount)
			.property("numBundles", &ofxOscBundle::getBundleCount)
	;
}

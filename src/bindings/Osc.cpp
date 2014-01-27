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

//--------------------------------------------------------------
// dummy class for argument types
struct OscArgType {};

//--------------------------------------------------------------
// luabind registration
luabind::scope Bindings::registerOsc() {
		
	using namespace luabind;
	
	return
				
		///////////////////////////////
		/// \section ofxOsc
		
		namespace_("osc") [
		
			// convenience functions
			def("send", &sendBang),
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
				.def("setRemoteEndpoint", &ofxOscMessage::setRemoteEndpoint)
				.def("getArgType", &ofxOscMessage::getArgType)
				.def("getArgTypeName", &ofxOscMessage::getArgTypeName)
				.def("getArgAsInt32", &ofxOscMessage::getArgAsInt32)
				.def("getArgAsFloat", &ofxOscMessage::getArgAsFloat)
				.def("getArgAsString", &ofxOscMessage::getArgAsString)
				.def("addIntArg", &ofxOscMessage::addIntArg)
				.def("addFloatArg", &ofxOscMessage::addFloatArg)
				.def("addStringArg", &ofxOscMessage::addStringArg)
				.def("getNumArgs", &ofxOscMessage::getNumArgs)
				.property("numArgs", &ofxOscMessage::getNumArgs),
				
			///////////////////////////////
			/// \section Bundle
			
			class_<ofxOscBundle>("Bundle")
				.def(constructor<>())
				.def("clear", &ofxOscBundle::clear)
				.def("addMessage", &ofxOscBundle::addMessage)
				.def("addBundle", &ofxOscBundle::addBundle)
				.def("getMessageAt", &ofxOscBundle::getMessageAt)
				.def("getBundleAt", &ofxOscBundle::getBundleAt)
				.def("getMessageCount", &ofxOscBundle::getMessageCount)
				.property("messageCount", &ofxOscBundle::getMessageCount)
				.def("getBundleCount", &ofxOscBundle::getBundleCount)
				.property("bundleCount", &ofxOscBundle::getBundleCount)
		]
	;
}

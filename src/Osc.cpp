/*
 * Copyright (c) 2012 Dan Wilcox <danomatika@gmail.com>
 *
 * BSD Simplified License.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 *
 * See https://github.com/danomatika/robotcowboy for documentation
 *
 */
#include "Osc.h"

#include "Global.h"
#include "poco/StringTokenizer.h"

//--------------------------------------------------------------
Osc::Osc() {
	sendAddress = "127.0.0.1";
	sendPort = 8880;
	receivePort = 9009;
}

//--------------------------------------------------------------
void Osc::setup() {
	sender.setup(sendAddress, sendPort);
	receiver.setup(receivePort);
}

//--------------------------------------------------------------
void Osc::update() {
	
	Global &global = Global::instance();
	
	// check for waiting osc messages
	while(receiver.hasWaitingMessages()) {
		
		ofxOscMessage msg;
		receiver.getNextMessage(&msg);
	
		// get the main destination address
		Poco::StringTokenizer tokenizer(msg.getAddress(), "/");
		Poco::StringTokenizer::Iterator iter = tokenizer.begin()+1;
		string dest = (*iter);
		
		// give to lua or pd
		if(dest == "visual") {
			global.scriptEngine.sendOsc(msg);
		}
		else if(dest == "audio") {
			global.audioEngine.sendOsc(msg);
		}
		else {
			ofLogWarning() << "Osc: Unhandled message: " << msg.getAddress();
		}
	}
}

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
	
	Adapted from https://github.com/danomatika/oscframework

==============================================================================*/
#pragma once

#include "OscObject.h"

/// a threaded osc receiver, add child OscObjects to process messages
class OscReceiver {

	public:

		OscReceiver(string rootAddress="");
		virtual ~OscReceiver();

		/// calls setup automatically
		OscReceiver(unsigned int port, string rootAddress="");

		/// setup the udp socket using the given port
		/// returns false if socket cannot be setup
		bool setup(unsigned int port);
		
		void update();

		/// start the listening thread, opens connection
		void start();

		/// stop the listening thread, closes connection
		void stop();
		
		/// attach osc objects to handle messages
		void addOscObject(OscObject *object);
		void removeOscObject(OscObject *object);

		/// is the thread running?
		bool isListening() {return true;}

		/// get port num
		unsigned int getPort();

		/// get/set the root address of this object
		void setOscRootAddress(std::string rootAddress);
		string& getOscRootAddress();

		// ignore incoming messages?
		void ignoreMessages(bool yesno);

	protected:

		/// root address of this object, something like "/root/test1/string2"
		string oscRootAddress;

	private:

		unsigned int m_port;
		ofPtr<ofxOscReceiver> m_receiver;
		bool m_bIsRunning, m_bIgnoreMessages;
		vector<OscObject*> _objectList;    /// list of osc objects
};

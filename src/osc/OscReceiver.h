/*==============================================================================

	Visual: a simple, osc-controlled graphics & scripting engine
  
	Copyright (c) 2013 Dan Wilcox <danomatika@gmail.com>

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program. If not, see <http://www.gnu.org/licenses/>.
	
	See https://github.com/danomatika/Visual for documentation
	
	Adapted from https://github.com/danomatika/oscframework

==============================================================================*/
#pragma once

#include "OscObject.h"

/// a threaded osc receiver, add child OscObjects to process messages
class OscReceiver {

	public:

		OscReceiver();
		virtual ~OscReceiver();

		/// calls setup automatically
		OscReceiver(unsigned int port);

		/// setup the udp socket using the given port
		/// returns false if socket cannot be setup
		bool setup(unsigned int port);

		/// start the listening thread, opens connection
		void start();

		/// stop the listening thread, closes connection
		void stop();
		
		/// attach osc objects to handle messages, does not delete
		void addOscObject(OscObject *object);
		void removeOscObject(OscObject *object);

		/// is the thread running?
		bool isListening() {return m_receiver != NULL;}

		/// get port num
		unsigned int getPort();

		/// ignore incoming messages?
		void ignoreMessages(bool yesno);

	protected:
	
		/// handles message
		void processMessage(const ofxOscMessage &message);
		
		/// wrapper to override and implement message processing
		class Receiver : public ofxOscReceiver {
			public:
				Receiver() : ofxOscReceiver(), receiver(NULL) {}
				OscReceiver *receiver;
			protected:
				void ProcessMessage(const osc::ReceivedMessage &m,
					const osc::IpEndpointName& remoteEndpoint);
		};
		friend Receiver;
		
		unsigned int m_port;
		ofPtr<Receiver> m_receiver;
		bool m_bIsRunning, m_bIgnoreMessages;
		vector<OscObject*> _objectList; //< list of osc objects
};

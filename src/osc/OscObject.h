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

#include "ofxOsc.h"

/// derive this class to add to an OscListener,
/// set the processing function to match messages
class OscObject {

	public:

		OscObject(string rootAddress="") : oscRootAddress(rootAddress) {}

		/// process attached objects, then call processOscMessage
		/// returns true if message handled
		bool processOsc(const ofxOscMessage& message);

		/// attach/remove an OscObject to this one
		void addOscObject(OscObject *object);
		void removeOscObject(OscObject *object);

		/// get/set the root address of this object
		void setOscRootAddress(string rootAddress);
		string& getOscRootAddress();
		void prependOscRootAddress(string prepend);

	protected:

		/// callback to implement, returns true if message handled
		virtual bool processOscMessage(const ofxOscMessage& message) {return false;}

		/// the root address of this object, aka something like "/root/test1/string2"
		string oscRootAddress;

	private:

		vector<OscObject*> _objectList;
};

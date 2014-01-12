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
#pragma once

#include "ofxOsc.h"

/// static class of useful utility functions
class Util {

	public:

		//// \section Path

		// returns the path to the executable
//		static string executablePath();

		/// \section OSC

//		// get a message argument as a number and store it in dest
//		static bool asNumber(int &dest, const ofxOscMessage& message, int argIndex);
//		static bool asNumber(unsigned int &dest, const ofxOscMessage& message, int argIndex);
//		static bool asNumber(float &dest, const ofxOscMessage& message, int argIndex);
//		
//		// get a message argument as a bool and store it in dest
//		static void asBool(bool &dest, const ofxOscMessage& message, int argIndex);
		
		/// try to get an argument as a given type, fail silently
		static bool tryBool(const ofxOscMessage &message, bool &dest, unsigned int at); // include numbers
		static bool tryChar(const ofxOscMessage &message, char &dest, unsigned int at); // includes ints
		static bool tryNumber(const ofxOscMessage &message, int &dest, unsigned int at);
		static bool tryNumber(const ofxOscMessage &message, unsigned int &dest, unsigned int at);
		static bool tryNumber(const ofxOscMessage &message, float &dest, unsigned int at);
		static bool tryNumber(const ofxOscMessage &message, double &dest, unsigned int at);
		static bool tryString(const ofxOscMessage &message, string &dest, unsigned int at);
};

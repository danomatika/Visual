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

==============================================================================*/
#pragma once

#include "DrawableObject.h"

class Script : public DrawableObject {

	public:

		Script(string name);
		Script(string name, string filename);

		bool loadFile(string filename="");

		void setup();
		void clear();
		
		/// getters / setters
		string getFilename() {return filename;}
		bool isLoaded() {return bLoaded;}
		
		string getType() {return "script";}
		
		bool allowsEarlySetup() {return false;}
		bool shouldAlwaysBeSetup() {return true;}
		bool shouldClearOnExit() {return true;}

	protected:

		/// osc callback
		bool processOscMessage(const ofxOscMessage& message) {return false;}

		string filename;
		bool bLoaded;
};

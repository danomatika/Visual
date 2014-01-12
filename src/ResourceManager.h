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

#include "ofMain.h"

class ResourceManager {

	public:

		~ResourceManager();

		void clear();

		/// fonts
		bool addFont(const string& name, const string& file, unsigned int size);
		void removeFont(const string& name);
		bool fontExists(const string& name);
		ofTrueTypeFont* getFont(const string& name);
		void clearFonts();
		
		/// images
		bool addImage(const string& name, const string& file);
		void removeImage(const string& name);
		bool imageExists(const string& name);
		ofImage* getImage(const string& name);
		void clearImages();

	protected:

		map<string,ofPtr<ofTrueTypeFont> > fonts;
		map<string,ofPtr<ofImage> > images;
};

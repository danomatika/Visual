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

#include "ofMain.h"

#define DEFAULT_FONT_SIZE	12

class ResourceManager {

	public:

		~ResourceManager();

		//void update();
		void clear();

		/// fonts
		bool addFont(const string& name, unsigned int size=DEFAULT_FONT_SIZE, string file="");
		void removeFont(const string& name, unsigned int size=DEFAULT_FONT_SIZE);
		bool fontExists(const string& name, unsigned int size=DEFAULT_FONT_SIZE);
		ofPtr<ofTrueTypeFont> getFont(const string& name, unsigned int size=DEFAULT_FONT_SIZE);
		void clearFonts();
		
		/// images
		bool addImage(const string& name, const string& file);
		void removeImage(const string& name);
		bool imageExists(const string& name);
		ofPtr<ofImage> getImage(const string& name);
		void clearImages();
		
		/// videos
		bool addVideo(const string& name, const string& file);
		void removeVideo(const string& name);
		bool videoExists(const string& name);
		ofPtr<ofVideoPlayer> getVideo(const string& name);
		void clearVideos();

	protected:
	
		// fonts can have multiple sizes, so each name is mapped to map of sizes
		typedef map<unsigned int,ofPtr<ofTrueTypeFont> > FontMap;
		struct FontSet {
			FontMap fonts;
			string filename;
		};
		map<string,FontSet> fonts;
		
		map<string,ofPtr<ofImage> > images;
		map<string,ofPtr<ofVideoPlayer> > videos;
};

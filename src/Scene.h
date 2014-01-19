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

#include "OscObject.h"
#include "objects/DrawableObject.h"

class Scene : public OscObject {

	public:

		Scene(string name);
		virtual ~Scene();

		/// add an object
		void addObject(DrawableObject* object);

		/// remove an object
		void removeObject(DrawableObject* object);
		
		/// clears (deletes) all the objects in the list
		void clearObjects();

		/// setup resources
		void setup();
		
		/// update objects
		void update();

		/// draw objects
		void draw();
		
		/// clear resources
		//void exit();
		
		/// Util
		string getName() {return name;}
		ofColor& getBackground() {return background;}
		void setBackground(ofColor &c) {background = c;}
		int getFps() {return fps;} // returns -1 if not set
		void setFps(unsigned int fps) {this->fps = fps;}

	private:

		bool bSetup; ///< make sure setup is only called once

		string name;
		vector<DrawableObject*> objects;
		ofColor background;
		int fps;
};

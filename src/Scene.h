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

		/// draw all the objects in the list
		void draw();
		
		/// Util
		string getName() {return _name;}
		ofColor& getBackground() {return _background;}
		void setBackground(ofColor &c) {_background = c;}
		int getFps() {return _fps;} // returns -1 if not set
		void setFps(unsigned int fps) {_fps = fps;}

	private:

		string _name;
		vector<DrawableObject*> _objectList;
		ofColor _background;
		int _fps;
};

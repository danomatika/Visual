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
	along with this program. If not, see <http://www.gnu.org/licenses/>.
	
	See https://github.com/danomatika/Visual for documentation

==============================================================================*/
#include "ofMain.h"

#include "App.h"
#include "Config.h"

int main(int argc, char *argv[]) {

	// create app before parsing
	Config::instance().app = ofPtr<App>(new App);

	// parse the commandline
	if(!Config::instance().parseCommandLine(argc, argv)) {
		return EXIT_FAILURE;
	}

	// setup graphics
	ofSetupOpenGL(1024, 576, OF_WINDOW);

	// main app loop
	ofRunApp(Config::instance().app);
	
	return EXIT_SUCCESS;
}

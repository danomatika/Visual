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
#include "Util.h"

// PATH

//#if defined TARGET_WIN32
//	#include <windows.h>
//#elif defined TARGET_OSX
//	#include <mach-o/dyld.h>
//#endif
//
//// from http://stackoverflow.com/questions/1023306/finding-current-executables-path-without-proc-self-exe
//string Util::executablePath() {
//	char path[1024];
//	
//	#if defined TARGET_WIN32
//		// Will contain exe path
//		HMODULE hModule = GetModuleHandle(NULL);
//		if(hModule != NULL) {
//			// When passing NULL to GetModuleHandle, it returns handle of exe itself
//			GetModuleFileName(hModule, path, (sizeof(path)));
//		}
//		else
//			return "";
//	#elif defined TARGET_OSX
//		uint32_t size = sizeof(path);
//		if(_NSGetExecutablePath(path, &size) != 0)
//			return "";
//	#else // LINUX
//		readlink("/proc/self/exe", path, sizeof(path));
//	#endif
//	
//	return (string) path;
//}

// OSC

//void Util::asNumber(int &dest, const ofxOscMessage& message, int at) {
//	if(message.getArgType(at) == OFXOSC_TYPE_INT32 ||
//	   message.getArgType(at) == OFXOSC_TYPE_INT64) {
//		&dest = message.getArgAsInt32(at);
//	}
//	else if(message.getArgType(at) == OFXOSC_TYPE_FLOAT) {
//		&dest = (int) message.getArgAsFloat(at);
//	}
//}
//
//void Util::asNumber(unsigned int &dest, const ofxOscMessage& message, int at) {
//	if(message.getArgType(at) == OFXOSC_TYPE_INT32 ||
//	   message.getArgType(at) == OFXOSC_TYPE_INT64) {
//		&dest = (unsigned int) message.getArgAsInt32(at);
//	}
//	else if(message.getArgType(at) == OFXOSC_TYPE_FLOAT) {
//		&dest = (unsigned int) message.getArgAsFloat(at);
//	}
//}
//
//void Util::asNumber(float &dest, const ofxOscMessage& message, int at) {
//	if(message.getArgType(at) == OFXOSC_TYPE_INT32 ||
//	   message.getArgType(at) == OFXOSC_TYPE_INT64) {
//		&dest = (float) message.getArgAsInt32(at);
//	}
//	else if(message.getArgType(at) == OFXOSC_TYPE_FLOAT) {
//		&dest = message.getArgAsFloat(at);
//	}
//}
//
//void Util::asBool(bool &dest, const ofxOscMessage& message, int at) {
//	if(message.getArgType(at) == OFXOSC_TYPE_INT32 ||
//	   message.getArgType(at) == OFXOSC_TYPE_INT64) {
//		&dest = (bool) message.getArgAsInt32(at);
//	}
//	else if(message.getArgType(at) == OFXOSC_TYPE_FLOAT) {
//		&dest = (bool) message.getArgAsFloat(at);
//	}
//}

bool Util::tryBool(const ofxOscMessage& message, bool &dest, unsigned int at) {
	if(message.getArgType(at) == OFXOSC_TYPE_INT32 ||
	   message.getArgType(at) == OFXOSC_TYPE_INT64) {
		dest = (bool) message.getArgAsInt32(at);
		return true;
	}
	else if(message.getArgType(at) == OFXOSC_TYPE_FLOAT) {
		dest = (bool) message.getArgAsFloat(at);
		return false;
	}
	return false;
}

bool Util::tryChar(const ofxOscMessage& message, char &dest, unsigned int at) {
	if(message.getArgType(at) == OFXOSC_TYPE_INT32 ||
	   message.getArgType(at) == OFXOSC_TYPE_INT64) {
		dest = (char) message.getArgAsInt32(at);
		return true;
	}
	return false;
}

bool Util::tryNumber(const ofxOscMessage& message, int &dest, unsigned int at) {
	if(message.getArgType(at) == OFXOSC_TYPE_INT32 ||
	   message.getArgType(at) == OFXOSC_TYPE_INT64) {
		dest = message.getArgAsInt32(at);
		return true;
	}
	else if(message.getArgType(at) == OFXOSC_TYPE_FLOAT) {
		dest = (int) message.getArgAsFloat(at);
		return false;
	}
	return false;
}

bool Util::tryNumber(const ofxOscMessage& message, unsigned int &dest, unsigned int at) {
	if(message.getArgType(at) == OFXOSC_TYPE_INT32 ||
	   message.getArgType(at) == OFXOSC_TYPE_INT64) {
		dest = (unsigned int) message.getArgAsInt32(at);
		return true;
	}
	else if(message.getArgType(at) == OFXOSC_TYPE_FLOAT) {
		dest = (unsigned int) message.getArgAsFloat(at);
		return false;
	}
	return false;
}

bool Util::tryNumber(const ofxOscMessage& message, float &dest, unsigned int at) {
	if(message.getArgType(at) == OFXOSC_TYPE_INT32 ||
	   message.getArgType(at) == OFXOSC_TYPE_INT64) {
		dest = (float) message.getArgAsInt32(at);
		return true;
	}
	else if(message.getArgType(at) == OFXOSC_TYPE_FLOAT) {
		dest = message.getArgAsFloat(at);
		return false;
	}
	return false;
}

bool Util::tryNumber(const ofxOscMessage& message, double &dest, unsigned int at) {
	if(message.getArgType(at) == OFXOSC_TYPE_INT32 ||
	   message.getArgType(at) == OFXOSC_TYPE_INT64) {
		dest = (double) message.getArgAsInt32(at);
		return true;
	}
	else if(message.getArgType(at) == OFXOSC_TYPE_FLOAT) {
		dest = (double) message.getArgAsFloat(at);
		return false;
	}
	return false;
}

bool Util::tryString(const ofxOscMessage& message, string &dest, unsigned int at) {
	if(message.getArgType(at) == OFXOSC_TYPE_STRING) {
		dest = message.getArgAsString(at);
		return true;
	}
	return false;
}

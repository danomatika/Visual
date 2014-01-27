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
#include "Bindings.h"

#include "ofxTween.h"

//--------------------------------------------------------------
float map7(float value, float inputMin, float inputMax, float outputMin, float outputMax, bool clamp, const ofxEasing & easing) {
	ofxTween::map(value, inputMin, inputMax, outputMin, outputMax, clamp, easing);
}

//--------------------------------------------------------------
float map8(float value, float inputMin, float inputMax, float outputMin, float outputMax, bool clamp, const ofxEasing & easing, ofxTween::ofxEasingType type) {
	ofxTween::map(value, inputMin, inputMax, outputMin, outputMax, clamp, easing, type);
}

//--------------------------------------------------------------
// luabind registration
luabind::scope Bindings::registerTween() {
		
	using namespace luabind;
	
	return
				
		///////////////////////////////
		/// \section ofxTween
		
		class_<ofxTween>("Tween")
			.def(constructor<>())
			.def(constructor<int,ofxEasing&,ofxTween::ofxEasingType,float,float,unsigned,unsigned>())
			
			.def("setParameters", (void(ofxTween::*)(int,ofxEasing&,ofxTween::ofxEasingType,float,float,unsigned,unsigned)) &ofxTween::setParameters)
			.def("setParameters", (void(ofxTween::*)(ofxEasing&,ofxTween::ofxEasingType,float,float,unsigned,unsigned)) &ofxTween::setParameters)
			
			.def("addValue", &ofxTween::addValue)
			.def("start", &ofxTween::start)
			.def("update", &ofxTween::update)
			
			.def("getTarget", &ofxTween::getTarget)
			.def("getDuration", &ofxTween::getDuration)
			.def("setDuration", &ofxTween::setDuration)
			
			.def("isRunning", &ofxTween::isRunning)
			.def("isDelaying", &ofxTween::isDelaying)
			.def("isCompleted", &ofxTween::isCompleted)
			
			.def("setFrameBasedAnimation", &ofxTween::setFrameBasedAnimation)
			
			.def_readwrite("id", &ofxTween::id)
			// ignoring end_E ofEvent for now
			
			.enum_("easingType") [
				value("easeIn", ofxTween::easeIn),
				value("easeOut", ofxTween::easeOut),
				value("easeInOut", ofxTween::easeInOut)
			]
			
			// static functions
			.scope [
				def("map", &map7),
				def("map", &map8)
			],
			
		///////////////////////////////
		/// \section ofxEasings
		
		class_<ofxEasingBack>("EasingBack")
			.def(constructor<>()),
			
		class_<ofxEasingBounce>("EasingBounce")
			.def(constructor<>()),

		class_<ofxEasingCubic>("EasingCubic")
			.def(constructor<>()),
			
		class_<ofxEasingElastic>("EasingElastic")
			.def(constructor<>()),
			
		class_<ofxEasingLinear>("EasingLinear")
			.def(constructor<>()),
			
		class_<ofxEasingQuart>("EasingQuart")
			.def(constructor<>()),
			
		class_<ofxEasingCirc>("EasingCirc")
			.def(constructor<>()),
			
		class_<ofxEasingExpo>("EasingExpo")
			.def(constructor<>()),
			
		class_<ofxEasingQuad>("EasingQuad")
			.def(constructor<>()),
			
		class_<ofxEasingQuint>("EasingQuint")
			.def(constructor<>()),
			
		class_<ofxEasingSine>("EasingSine")
			.def(constructor<>())
	;
}

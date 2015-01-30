/*
	2015 Dan Wilcox <danomatika@gmail.com>
*/

%module osc
%{
#include "ofxOsc.h"
#include "Osc.h"
%}

%include <attribute.i>

// STL types
%include <std_string.i>

// ----- Renaming -----

// strip "ofxOsc" prefix from classes
%rename("%(strip:[ofxOsc])s", %$isclass) "";

// strip "ofxOsc" prefix from global functions & make first char lowercase
%rename("%(regex:/ofxOsc(.*)/\\l\\1/)s", %$isfunction) "";

// strip "OFXOSC_" from constants & enums
%rename("%(strip:[OFXOSC_])s", %$isconstant) "";
%rename("%(strip:[OFXOSC_])s", %$isenumitem) "";

////////////////////////////////////////////////////////////////////////////////
// ----- BINDINGS --------------------------------------------------------------

// ----- OSC -----------------------------------------------------------------

// only need Message and Bundle
%include "ofxOsc/src/ofxOscMessage.h"
%include "ofxOsc/src/ofxOscBundle.h"

%attribute(ofxOscMessage, int, numArgs, getNumArgs);
%attribute(ofxOscBundle, int, messageCount, getMessageCount);
%attribute(ofxOscBundle, int, bundleCount, getBundleCount);

// Osc helpers
%include "../src/bindings/Osc.h"

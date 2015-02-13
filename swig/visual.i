/*
	2015 Dan Wilcox <danomatika@gmail.com>
*/

%module visual
%{
#include "Visual.h"
%}

%include <attribute.i>

// STL types
%include <stl.i>
%include <std_string.i>

// OF types we need to know about
//%import(module="of") "ofxLua/swig/openFrameworks.i"
%import(module="of") "types/ofColor.h"
%import(module="of") "types/ofPoint.h"
%import(module="of") "utils/ofConstants.h"

////////////////////////////////////////////////////////////////////////////////
// ----- BINDINGS --------------------------------------------------------------

// Visual helpers
%include "../src/bindings/Visual.h"

// forward declares to silece warning
%ignore OscObject;
class OscObject {};

// ----- Config -----------------------------------------------------------------

// this is a dummy class, you shouldn't be able to create it directly,
// it's only for access to the config variables
%nodefaultctor Config;
%nodefaultdtor Config; 

// define intended interface here instead of lots of ignores
class Config {

	public:

		void print();
		
		bool isPlaylist;
		unsigned int listeningPort;
		std::string sendingIp;
		unsigned int sendingPort; 
		
		std::string notificationAddress;
		std::string deviceAddress;
		unsigned int connectionId;

		unsigned int renderWidth, renderHeight;
		void setRenderSize(unsigned int w, unsigned int h);
		bool fullscreen;
		
		bool setupAllScenes;
		bool showSceneNames;
};

%attributestring(Config, string, baseAddress, getBaseAddress, setBaseAddress);

// ----- Scene -----------------------------------------------------------------

%rename(add) Scene::addObject;
%rename(remove) Scene::removeObject;
%rename(clear) Scene::clearObject;

// lua gives up ownership to C++
%apply SWIGTYPE *DISOWN { DrawableObject* object_disown };

class Scene {

	public:

		Scene(std::string name);

		void addObject(DrawableObject* object_disown);
		void removeObject(DrawableObject* object);
		void clearObjects();

		void setup(bool earlySetup=false);
		void draw();
};

%attribute(Scene, bool, slideshow, getSlideshow, setSlideshow);
%attributestring(Scene, std::string, name, getName);
%attribute(Scene, int, fps, getFps, setFps);
%attribute(Scene, ofColor&, background, getBackground, setBackground);

// ----- DrawableObject --------------------------------------------------------

%ignore DrawableObject::setup;
%ignore DrawableObject::update;

class DrawableObject {

	public:

		DrawableObject(std::string objectName);

		virtual void draw();
		virtual void draw(int x, int y);
		virtual void draw(int x, int y, unsigned int w, unsigned int h);
		virtual void clear();

		virtual void setSize(unsigned int w, unsigned int h);
};

%attribute(DrawableObject, bool, visible, getVisible, setVisible);
%attribute(DrawableObject, ofColor&, color, getColor, setColor);
%attributestring(DrawableObject, std::string, name, getName);
%attributestring(DrawableObject, std::string, type, getType);

class DrawableFrame : public DrawableObject {

	public:

		DrawableFrame(std::string objectName);
		DrawableFrame(std::string objectName, unsigned int frameTime);
};

%attribute(DrawableFrame, unsigned int, frameTime, getFrameTime, setFrameTime);

// ----- Bitmap ----------------------------------------------------------------

class Bitmap : public DrawableFrame {

	public:

		Bitmap(std::string name, unsigned int w, unsigned int h);
		Bitmap(unsigned int frameTime, unsigned int w, unsigned int h);
};

%attributestring(Bitmap, std::string, bitmap, getBitmap, setBitmap);
%attribute(Bitmap, ofPoint&, position, getPos, setPos);
%attribute(Bitmap, unsigned int, width, getWidth, setWidth);
%attribute(Bitmap, unsigned int, height, getHeight, setHeight);
%attribute(Bitmap, bool, center, getDrawFromCenter, setDrawFromCenter);

// ----- Image -----------------------------------------------------------------

%rename(Image2) Image;

class Image : public DrawableFrame {

	public:

		Image(std::string name);
		Image(std::string name, std::string filename);
		Image(unsigned int frameTime, std::string filename);

		bool loadFile(std::string filename="");

		void setup();
};

%attribute(Image, ofImage&, image, getImage);
%attribute(Image, bool, loaded, isLoaded);
%attributestring(Image, std::string, filename, getFilename);
%attribute(Image, ofPoint&, position, getPos, setPos);
%attribute(Image, unsigned int, width, getWidth, setWidth);
%attribute(Image, unsigned int, height, getHeight, setHeight);
%attribute(Image, bool, center, getDrawFromCenter, setDrawFromCenter);

// ----- Line ------------------------------------------------------------------

class Line : public DrawableObject {

	public:

		Line(std::string name);
		Line(std::string name, int x1, int y1, int x2, int y2);
		Line(std::string name, ofPoint& p1, ofPoint& p2);
};

%attribute(Line, ofPoint&, position1, getPos1, setPos1);
%attribute(Line, ofPoint&, position2, getPos2, setPos2);

// ----- Pixel -----------------------------------------------------------------

class Pixel : public DrawableObject {

	public:

		Pixel(std::string name);
		Pixel(std::string name, int x, int y);
};

%attribute(Pixel, ofPoint&, position, getPos, setPos);

// ----- Rectangle -------------------------------------------------------------

%rename(Rectangle2) Rectangle;

class Rectangle : public DrawableObject {

	public:

		Rectangle(std::string name);
		Rectangle(std::string name, int x, int y, unsigned int w, unsigned int h);
		Rectangle(std::string name, ofPoint &p, unsigned int w, unsigned int h);
};

%attribute(Rectangle, ofPoint&, position, getPos, setPos);
%attribute(Rectangle, unsigned int, width, getWidth, setWidth);
%attribute(Rectangle, unsigned int, height, getHeight, setHeight);
%attribute(Rectangle, bool, filled, getFilled, setFilled);
%attribute(Rectangle, bool, center, getDrawFromCenter, setDrawFromCenter);

// ----- Script ----------------------------------------------------------------

class Script : public DrawableObject {

	public:

		Script(std::string name);
		Script(std::string name, std::string filename);

		bool loadFile(std::string filename="");

		void setup();
};

%attribute(Script, bool, loaded, isLoaded);
%attributestring(Script, std::string, filename, getFilename);

// ----- Sprite ----------------------------------------------------------------

%rename(add) Sprite::addFrame;
%rename(remove) Sprite::removeFrame;
%rename(clear) Sprite::clearFrames;

// lua gives up ownership to C++
%apply SWIGTYPE *DISOWN { DrawableFrame* frame_disown };

class Sprite : public DrawableObject {

	public:

		Sprite(std::string name);

		void addFrame(DrawableFrame* frame_disown);
		void removeFrame(DrawableFrame* frame);
		void clearFrames();

		void nextFrame();
		void prevFrame();
		void gotoFrame(unsigned int num);
		void gotoFrame(std::string name);
		unsigned int getCurrentFrame();

		void setup();

		void setAnimation(bool animate, bool loop, bool pingPong);
};

%attribute(Sprite, unsigned int, frame, getCurrentFrame);
%attribute(Sprite, ofPoint&, position, getPos, setPos);
%attribute(Sprite, unsigned int, width, getWidth, setWidth);
%attribute(Sprite, unsigned int, height, getHeight, setHeight);
%attribute(Sprite, bool, animate, getAnimate, setAnimate);
%attribute(Sprite, bool, loop, getLoop, setLoop);
%attribute(Sprite, bool, pingpong, getPingPong, setPingPong);
%attribute(Sprite, bool, center, getDrawFromCenter, setDrawFromCenter);
%attribute(Sprite, bool, overlay, getDrawAllLayers, setDrawAllLayers);

// ----- Text ------------------------------------------------------------------

class Text : public DrawableObject {

	public:

		Text(std::string name);
		Text(std::string name, std::string filename);
		Text(std::string name, unsigned int size);
		Text(std::string name, std::string filename, unsigned int size);
		
		void setup();

		ofTrueTypeFont& getFont() {return *font;}
		bool isFontLoaded() {return font->isLoaded();}
		
		string getFontFilename() {return fontFilename;}
		unsigned int getFontSize() {return fontSize;}
};

%attribute(Text, ofTrueTypeFont&, font, getFont);
%attribute(Text, bool, fontLoaded, isFontLoaded);
%attributestring(Text, std::string, fontFilename, getFontFilename);
%attribute(Text, unsigned int, fontSize, getFontSize);
%attribute(Text, ofPoint&, position, getPos, setPos);
%attributestring(Text, std::string, text, getText, setText);
%attribute(Text, bool, center, getDrawFromCenter, setDrawFromCenter);

// ----- Video -----------------------------------------------------------------

class Video : public DrawableObject {

	public:

		Video(std::string name);
		Video(std::string name, std::string filename);

		bool loadFile(std::string filename="");

		void setup();

		ofVideoPlayer& getVideo();
		bool isLoaded();

		string getFilename();
};

%attribute(Video, ofVideoPlayer&, video, getVideo);
%attribute(Video, bool, loaded, isLoaded);
%attributestring(Video, std::string, filename, getFilename);
%attribute(Video, bool, play, getPlay, setPlay);
%attribute(Video, float, volume, getVolume, setVolume);
%attribute(Video, float, speed, getSpeed, setSpeed);
%attribute(Video, ofLoopType, loop, getLoop, setLoop);
%attribute(Video, ofPoint&, position, getPos, setPos);
%attribute(Video, unsigned int, width, getWidth, setWidth);
%attribute(Video, unsigned int, height, getHeight, setHeight);
%attribute(Video, bool, center, getDrawFromCenter, setDrawFromCenter);

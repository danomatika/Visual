Visual
======

**_This project is dead. See [loaf](https://github.com/danomatika/loaf) instead._**

a simple scene manager and general purpose OF + Lua interpreter controllable over Open Sound Control

directly inspired by [Love2d](http://www.love2d.org/), [Fluxus](http://www.pawfal.org//fluxus), etc 

GPL v3  
Copyright (c) [Dan Wilcox](danomatika.com) 2014

DESCRIPTION
-----------

This is basically a more advanced version of [rc-visual](https://github.com/danomatika/rc-visual) built for the [robotcowboy: Onward to Mars](http://robotcowboy.com/onwardtomars) project.

A current OSX build can be found at: <http://docs.danomatika.com/Visual.app.zip>

Simply create lua scripts using the [ofxLua api bindings](https://github.com/danomatika/ofxLua/tree/master/src/bindings) and run them in Visual by either dragging and dropping the script, setting it as a command line argument, or a script within a playlist/scene lua script.

For a detailed info on the OF Lua bindings see: <https://github.com/danomatika/ofxLua#of-api-bindings>

Nutshell: 

* of* things become of.*: ofRect(100, 100, 200, 200); -> of.rect(100, 100, 200, 200)
* enums have the first two underscores replaced by periods OF_KEY_DOWN -> of.KEY.DOWN & OF_ALIGN_HORZ_LEFT -> of.ALIGN.HORZ_LEFT
* create class instances like so: image = of.Image() <- default constructor & color = of.Color(255) <- constructor with variables ... not much diff from C++

See the ofxLua example scripts for now on how the callback functions are laid out: <https://github.com/danomatika/ofxLua/tree/master/luaExample/bin/data/scripts>

There is a built in OSC server, scene manager, scene types, etc but I don't have time to document all of that right now. Will get to that later ... 

See the example scripts & media in the `/res` folder. `example.lua` demonstrates a scene manager playlist, adding scenes and scene objects. `test.lua` is a simple lua script using the OF Lua bindings.

BUILD REQUIREMENTS
------------------

The following open frameworks addons are required:

* [ofxAppUtils](https://github.com/danomatika/ofxAppUtils)
* [ofxLua](https://github.com/danomatika/ofxLua)
* [ofxGLEditor](https://github.com/Akira-Hayasaka/ofxGLEditor)
* ofxOsc (included with openFrameworks)

USAGE
-----

The application has a full help usage printout, use -h or --help.

Basic app usage:

* run a .lua script onto the app to run said script
* CMD+R reloads the current script so you can edit in a text editor at the same time
* script errors are drawn to the screen

For commandline usage on OSX, I have the following alias in my ~/.bash_profile and coped Visual.app into my `/Applications` folder:

    alias visual="/Applications/Visual.app/Contents/MacOS/Visual $1"

This way I can run a lua script from anywhere:

    visual cool_script.lua

CMD+F toggles fullscreen

NOTES
-----

### Xcode settings after regenerating roject

Uncheck "Allow debugging when using document Versions Browser" in Scheme Run tab to disable debug commandline argument which causes parse failure.

Drag `bin/data` folder into project as a folder reference so resources will be built into the app bundle.

Add data folder to "Copy Files" build phase. Install to Resources folder.

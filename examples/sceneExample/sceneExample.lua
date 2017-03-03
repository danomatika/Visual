-- a config script while defines Visual scenes, objects, & settings

-- get global settings
config = visual.getConfig()

-- tell Visual this is a playlist file (important!)
config.isPlaylist = true

-- hide the editor when loading this playlist?
config.hideEditor = true

-- OSC receive settings
config.listeningPort = 9999

-- OSC send settings
config.sendingIp = "127.0.0.1"
config.sendingPort = 8880

--[[
  specify the OSC addresses when sending notifications and device (keyboard)
  events

  connectionId is sent with notifications to differentiate between
  multiple rc-visual instances
--]]
config.notificationAddress = "/pd/visual"
config.deviceAddress = "/pd/devices"
config.connectionId = 0

-- specify render (& window) size (default: 640 480)
config:setRenderSize(640, 480)

-- start in fullscreen? (default: false)
config.fullscreen = false

-- setup all the scenes after this file is loaded? (default: true)
-- or setup each scene when it is first run (false)
config.setupAllScenes = false

-- show the scene name when switching (default: false)
config.showSceneNames = true

-- this file is divided up into multiple scenes which can be switched between

-- graphics primitives
scene = visual.Scene("primitives")

	-- set the background
	scene.background:set(0)
	
	-- a line
	line = visual.Line("line")
		line.position1:set(200, 400)
		line.position2:set(580, 400)
		line.color:set(255, 0, 0)
		line.visible = true
	scene:add(line)
	
	-- a rectangle
	rect = visual.Box("rect")
		rect.position:set(100, 100)
		rect.width = 200
		rect.height = 260
		rect.color:set(0, 0, 255)
		rect.visible = true
	scene:add(rect)

visual.addScene(scene)

-- bitmaps and sprites
scene = visual.Scene("bitmap&sprite")

	scene.background:set(64, 64, 64)
	
	-- a simple, single color bitmap, stretched to fit a given size
	bitmap = visual.Bitmap("bitmap", 5, 5)
		bitmap.position:set(250, 500)
		bitmap:setSize(500, 500)
		bitmap.center = true
		bitmap.color:set(200)
		bitmap.visible = true
		--[[
			draw the bitmap in the frame
			 - '*': a filled pixel
			 - '_': an empty pixel
			 total number of pixels must match width/height in visual.Bitmap()
		
			note how the multiline string is denoted without "
		--]]
		bitmap.bitmap = [[
			--*--
			--*--
			*****
			--*--
			--*--
		]]
	scene:add(bitmap)

	-- a sprite is a sequence of bitmaps and/or images
	sprite = visual.Sprite("sprite1")
		sprite.position:set(320, 240)
		sprite.animate = true
		sprite.loop = true
		sprite.pingpong = true
		sprite.center = true
		sprite.overlay = false
		-- note frametime in milliseconds instead of name for animation
		bitmap = visual.Bitmap(1000, 5, 5)
			bitmap:setSize(50, 50)
			bitmap.color:set(0, 200, 0)
			bitmap.bitmap = [[
				***--
				***--
				*****
				--***
				--***
			]]
		sprite:add(bitmap)
		bitmap = visual.Bitmap(1000, 5, 5)
			bitmap:setSize(50, 50)
			bitmap.color:set(255)
			bitmap.bitmap = [[
				--*--
				--*--
				*****
				--*--
				--*--
			]]
		sprite:add(bitmap)
		bitmap = visual.Bitmap(1000, 5, 5)
			bitmap:setSize(50, 50)
			bitmap.color:set(0, 0, 200)
			bitmap.bitmap = [[
				-----
				--*--
				--*--
				--*--
				-----
			]]
		sprite:add(bitmap)
	scene:add(sprite)

	-- this is a sprite using images
	sprite = visual.Sprite("sprite1")
		sprite.position:set(480, 120)
		sprite:setSize(200, 200)
		sprite:setAnimation(true, true, true) -- animate, loop, & pingpong
		sprite.center = true
		sprite.overlay = false
		-- note frametime in milliseconds instead of name for animation
		sprite:add(visual.Photo(100, "media/animation/frame1.gif"))
		sprite:add(visual.Photo(100, "media/animation/frame2.gif"))
		sprite:add(visual.Photo(100, "media/animation/frame3.gif"))
		sprite:add(visual.Photo(100, "media/animation/frame4.gif"))
		sprite:add(visual.Photo(100, "media/animation/frame5.gif"))
		sprite:add(visual.Photo(100, "media/animation/frame6.gif"))
		sprite:add(visual.Photo(200, "media/animation/frame7.gif"))
	scene:add(sprite)

visual.addScene(scene)

-- images and text
scene = visual.Scene("image&text")
	scene.background:set(64)

	-- slideshow mode only shows one drawable object at a time,
	-- cycle through them using OSC commands
	scene.slideshow = true

	-- an image, rescaled when loaded to fit given size
	image = visual.Photo("image1", "../icon.png")
		image.position:set(100, 50)
		image:setSize(50, 50)
		image.center = false
		image.visible = true
	scene:add(image)

	-- image files can be reused as the filename is used as a unique id
	-- for reusing currently loaded images
	image = visual.Photo("image2", "../icon.png")
		image.position:set(150, 100)
		image:setSize(50, 50)
		image.center = false
		image.visible = true
	scene:add(image)
	
	image = visual.Photo("image3", "../icon.png")
		image.position:set(200, 150)
		image:setSize(100, 100)
		image.center = false
		image.visible = true
	scene:add(image)

	-- draw some text, uses default font & size
	text = visual.Text("text1")
		text.position:set(10, 290)
		text.center = false
		text.color:set(200)
		text.visible = true
		text.text = "this is a test"
	scene:add(text)

	-- default font with size
	text = visual.Text("text2", 18)
		text.position:set(10, 330)
		text.center = false
		text.color:set(200, 255, 127)
		text.visible = true
		text.text = "another test, should be bigger"
	scene:add(text)

	-- load a local font with size
	text = visual.Text("text3", "media/DejaVuSansMono.ttf", 32)
		text.position:set(320, 410)
		text.center = true
		text.color:set(255, 0, 255, 200)
		text.visible = true
		text.text = "a different, local font"
	scene:add(text)

visual.addScene(scene)

-- video
scene = visual.Scene("video")
	scene.background:set(0)

	video = visual.Video("video1", "media/test_clip.mov")
		video.position:set(320, 240)
		video.center = true
		video.play = true
		video.volume = 0
		video.speed = 2
		video.loop = of.LOOP_PALINDROME
	scene:add(video)

	-- reuse the first movie
	video2 = visual.Video("video1 clone", "media/test_clip.mov")
		video2:setSize(160, 90) -- make it smaller
		-- play, volume, & speed have already been set
	scene:add(video2)

visual.addScene(scene)

-- script
scene = visual.Scene("script")
	
	-- scripts have no options, only use 1 per scene as proceeding scripts will
	-- overwrite preceeding ones
	script = visual.Script("script", "script.lua")
	scene:add(script)

visual.addScene(scene)

-- same as the above via a quick helper,
-- both scene and script object share the same name
visual.addScript("script", "script.lua")

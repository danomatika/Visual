-- settings
config = visual.getConfig()

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

-- this file is divided up into multiple scenes which can be switched between

-- graphics primitives
scene = visual.Scene("scene1")

	-- set the background
	scene.background:set(0)
	
	-- a single pixel
	pixel = visual.Pixel("pixel")
		pixel.position:set(10, 10)
		pixel.color:set(255)
		pixel.visible = true
	scene:add(pixel)
	
	-- a line
	line = visual.Line("line")
		line.position1:set(200, 400)
		line.position2:set(580, 400)
		line.color:set(255, 0, 0)
		line.visible = true
	scene:add(line)
	
	-- a rectangle
	rect = visual.Rectangle("rect")
		rect.position:set(100, 100)
		rect.width = 200
		rect.height = 260
		rect.color:set(0, 0, 255)
		rect.visible = true
	scene:add(rect)

visual.addScene(scene)

-- bitmaps and sprites
scene = visual.Scene("scene2")

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
		sprite:add(visual.Image(100, "animation/frame1.gif"))
		sprite:add(visual.Image(100, "animation/frame2.gif"))
		sprite:add(visual.Image(100, "animation/frame3.gif"))
		sprite:add(visual.Image(100, "animation/frame4.gif"))
		sprite:add(visual.Image(100, "animation/frame5.gif"))
		sprite:add(visual.Image(100, "animation/frame6.gif"))
		sprite:add(visual.Image(200, "animation/frame7.gif"))
	scene:add(sprite)

visual.addScene(scene)

-- images and text
scene = visual.Scene("scene3")
	scene.background:set(64)

	-- an image, rescaled when loaded to fit given size
	image = visual.Image("image1", "icon.png")
		image.position:set(100, 100)
		image:setSize(50, 50)
		image.center = false
		image.visible = true
	scene:add(image)

	-- image files can be reused as the filename is used as a unique id
	-- for reusing currently loaded images
	image = visual.Image("image2", "icon.png")
		image.position:set(150, 150)
		image:setSize(50, 50)
		image.center = false
		image.visible = true
	scene:add(image)
	
	image = visual.Image("image3", "icon.png")
		image.position:set(200, 200)
		image:setSize(100, 100)
		image.center = false
		image.visible = true
	scene:add(image)

	-- draw some text
	text = visual.Text("text")
		text.position:set(320, 240)
		text.center = true
		text.color:set(200)
		text.visible = true
		text.text = "this is a test"
	scene:add(text)

visual.addScene(scene)

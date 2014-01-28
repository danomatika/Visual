require 'Hello'

-- global vars
time = 0.0
increment = 0.01

function setup()

	print "setup was called"
	of.setCircleResolution(50)

end

function update()

	of.background(255, 200, 200)

end

function draw()

	local n = of.noise(time)
	local size =  n * of.getWidth()/4

	time = time + increment

	of.setColor(255, 0, 0)
	of.circle(of.getWidth()/2, of.getHeight()/2, size, size)

	--osc.sendFloat("/audio/circle/size", n)
end

function exit()
	print "exit was called"
end

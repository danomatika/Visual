angle = 0
bg = of.Color(100, 220, 76)
color = of.Color(255, 100, 100)

function setup()
	of.setCircleResolution(50)
	of.hideCursor()
end

function draw()
	of.background(bg)
	of.translate(of.getWidth()/2, of.getHeight()/2)
	local s = 2.5*of.map(math.sin(angle), -1, 1, 0, 1)
	of.scale(s, s)
	of.setColor(color)	
	of.drawCircle(0, 0, 100, 100)
	angle = angle+0.0125
end

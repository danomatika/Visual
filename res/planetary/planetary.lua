-- the satellites are spinning...

require "Satellite"

sats = {}

function setup()
	of.setCircleResolution(100)
	of.setRectMode(of.RECTMODE_CENTER)
	for i=1,10 do
		sats[i] = Satellite(i * (of.radToDeg(of.TWO_PI/10)))
	end
end

function draw()
	of.pushMatrix()

	of.translate(of.getWidth()/2, of.getHeight()/2)

	of.setColor(220, 200, 100)
	of.drawCircle(0, 0, 100)	

	for i=1,#sats do
		sats[i]:draw()	
	end
	of.popMatrix()
end

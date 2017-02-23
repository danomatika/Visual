-- a Satellite class

Satellite = class()

function Satellite:__init(angle)
	self.pos = of.Point()
	self.angles = of.Point()
	self.angles.z = angle or 0
end

function Satellite:draw()
	
	of.pushMatrix()
	
	of.rotateZ(self.angles.z)
	of.translate(220, 0)
	of.rotateX(-self.angles.z)	
	of.rotateY(-self.angles.y)
	

	-- body
	of.setColor(180)
	of.drawBox(20)
	
	-- solar panels
	of.setColor(40, 40, 220)
	of.drawRectangle(-20, 0, 20, 60)		
	of.drawRectangle(20, 0, 20, 60)

	of.popMatrix()

	self.angles.y = self.angles.y + 1.5
	self.angles.z = self.angles.z + 0.1
end

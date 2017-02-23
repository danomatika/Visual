------------------------------
-- simple Circle class

Circle = class()

function CrazyCircle:__init(x, y, w, h)
    self.x = x
    self.y = y
    self.w = w
    self.h = h
    self.color = of.Color(of.random(50, 255), of.random(50, 255), of.random(50, 255))
end

function CrazyCircle:update()
    
end

function CrazyCircle:draw()
    of.setColor(self.color)
    of.noFill()
    of.drawCircle(self.x, self.y, math.sin(count+of.TWO_PI)*self.w)
end

-------------------------------
-- small script which uses the circle class

count = 0
circles = {}

function setup()
    of.background(150)
    of.setCircleResolution(3)

    for i = 1,10 do
        circles[i] = Circle(of.random(200, of.getWidth()-200),
                            of.random(200, of.getHeight()-200),
                            200, 200)
    end

    print(tostring(#circles).." circles")
end

function update()
    count = count+0.05

    for i = 1,#circles do
        circles[i]:update()
    end
end

function draw()
    for i = 1,#circles do
        circles[i]:draw()
    end
end

function keyPressed(key)
    if key == string.byte("l") then
		of.setCircleResolution(2)
	elseif key == string.byte("t") then
        of.setCircleResolution(3)
    elseif key == string.byte("q") then
        of.setCircleResolution(4)
    elseif key == string.byte("c") then
        of.setCircleResolution(50)
    end
end

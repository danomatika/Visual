-- this is a test scipt module loaded by script.lua
print("hello world")

color = of.Color(255)
color.r = 200
color.g = 100
color.b = 0
print(color)

point = of.Point(10, 10, 10)
print(point)

rect = of.Rectangle(10, 10, 10, 10)
rect.x = 20
rect.y = 20
print(rect.x.." "..rect.y)

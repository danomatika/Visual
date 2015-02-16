--[[
	Visual: a simple, osc-controlled graphics & scripting engine
  
	Copyright (c) 2013 Dan Wilcox <danomatika@gmail.com>
]]

-- override size with render size functions
of.getWidth = visual.getRenderWidth
of.getHeight = visual.getRenderHeight

-- custom print to hook into C++
function print(...)
	local s = ""
	for i,v in ipairs(arg) do
		s = s .. tostring(v)
	end
	visual.print(s)
end

-- variable argument OSC
function osc.send(...)
    local msg = osc.Message()
    for i,v in ipairs(arg) do
        if i == 1 then
            if type(v) == "string" then
                msg:setAddress(v)
            else
                print("error, first argument is not a string")
            end
        else
            if type(v) == "number" then
                msg:addFloatArg(v)
            elseif type(v) == "string" then
                msg:addStringArg(v)
            else
                print("skipping " .. type(v) .. " argument")
            end
        end
    end
    osc.sendMessage(msg)
end

-- print the bindings from each module table

function printTable(o,h)
	if type(o) == "table" then
		local s = ""
		for k,v in pairs(o) do
			if type(v) == 'table' then
				s = s..printTable(v,h.."."..k)
			elseif h ~= nil then
				s = s..h.."."..k.."\n"
			else
				s = s..k.."\n"
			end
		end
		return s
	else
		return tostring(o)
	end
end

print(printTable(of, "of"))
print(printTable(visual, "visual"))
print(printTable(osc, "osc"))

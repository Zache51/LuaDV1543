msg = "a "

function keyHandler(code)
	local var = code
	if(var == 57) then
		msg = "testing"
	else
		msg = "a "
	end
end

function print()
	printC(msg);
end